//WilkieRobot.cpp - Robot Control Driver.
//Written By Phil Wilkins April 2015
// Version 0.1 - April - 2015 - initial test basic motor control
// Version 0.2 - May - 2015 - Added compass control and threading for bearing readout


#include "BCM2835.h"
#include "CMPS11.h"
#include <string.h>
#include <stdio.h>
#include <pthread.h>

using namespace std;

bool inputLock=false;

struct thread_data
{
	int threadid;
	I2C *i2cptr;
	Orientation *Compass;
};

void SendCommand(I2C &i2c, int cmd, int speed)
{
	int bytes_written=i2c.writeReg8(cmd,speed);
	if(bytes_written==-1)
	{
		std::cout << "Error Sending Command - Write Failed." 
						 << std::endl;
		return;
	}
	else
	{
		std::cout << "\033[0;40HWritten " << bytes_written << " bytes:Command Sent." 
				<< std::endl;
		usleep(500000);
	}
return;
}

void *displayBearing(void *threadarg)
{
	float bearing=0.0f;
	int Sensordata=0,SensorStatus=0, SensorPos=0;
	int i=0;
	struct thread_data *recieved_data;
	
	recieved_data=(struct thread_data *)threadarg;
	cout.precision(3);
	for(;;)
	{
		if(!inputLock)
		{
			bearing=recieved_data->Compass->m_getBearing16();
			usleep(25000);
			Sensordata=recieved_data->i2cptr->readReg16(60);
			SensorStatus=Sensordata & 0xFF;
			SensorPos=(Sensordata >> 8) & 0xFF;			
			usleep(25000);			
			cout << "\e[0;32;40m\033[0;0fBearing:" << bearing << " Barrier Status:" << SensorStatus << 
							" Sensor Angle:" << SensorPos << " ";
			cout << "\033[20;0f";
			i=0;	
		}
		else
		{
			if(i!=2) cout << "\033[0;0fQuery:Pause  ";	
			cout << "\033[20;0f";
			i=2;
			usleep(5000);	
		}

	}
pthread_exit(NULL);
}

void *displayMenu(void *threadarg)
{
	int speed=0;
	char selection='0',exitCal='0';
	bool exitLoop=false;
	struct thread_data *recieved_data;
	
	recieved_data=(struct thread_data *)threadarg;
	while(!exitLoop)
	{

		system("clear");
		cout << "\033[2;0f";	
		cout << "\e[1;37;40m\033[3;0fWILKIEROBOT Controller v0.2" << endl;
		cout << "\033[4;0f--------------------------------" << endl;	
		cout << "\033[5;0fGPIO and I2C configured - Testing GPIO Output:" << endl;
		cout << "\033[6;0fEnabling GPIO I2C Pins on the PI." << endl;	
		cout << "\033[7;0fSetting Slave Address of the Arduino:" << endl;	
		cout << "\033[8;0fBaud Set to 90kHz to prevent stretching.." << endl;	
		//recieved_data->Compass->showSoftVersion();
		cout << "\e[0;36;40m\033[9;0fSelect Mode:" << endl << 
			"\033[10;0f(1) Set Speed" << endl <<
			"\033[11;0f(2) Move Forwards" << endl <<
			"\033[12;0f(3) Move Backwards" << endl <<
			"\033[13;0f(4) Move Left" << endl <<	
			"\033[14;0f(5) Move Right" << endl <<
			"\033[15;0f(6) Stop" << endl <<
			"\033[16;0f(7) Quit" << endl <<
			"\033[17;0f(8) Calibrate Compass" << endl <<
			"\033[18;0f(9) Restore Default Compass Settings" << endl;
	selection=getchar();
		
		switch(selection)
		{

			case '1':		inputLock=true;
						cout << "\033[19;0fEnter speed from 0 - 255:" << endl;
						cin >> speed;
						inputLock=false;
					break;  
			case '2':		inputLock=true;
						usleep(50);
						SendCommand(*recieved_data->i2cptr,10 & 0xFF,speed);
						inputLock=false;
					break;
			case '3':		inputLock=true;
						usleep(50);
						SendCommand(*recieved_data->i2cptr,20 & 0xFF,speed);
						inputLock=false;
					break;
			case '4':		inputLock=true;
						usleep(50);
						SendCommand(*recieved_data->i2cptr,30 & 0xFF,speed);
						inputLock=false;
					break;
			case '5':		inputLock=true;
						usleep(50);
						SendCommand(*recieved_data->i2cptr,40 & 0xFF,speed);
						inputLock=false;
					break;
			case '6':		inputLock=true;
						usleep(50);
						SendCommand(*recieved_data->i2cptr,50 & 0xFF,speed);
						inputLock=false;
					break;
			case '7':		exitLoop=true;
					break;			
			case '8':		cout << "\033[0;45f Press q to exit Calibration" << endl;
						inputLock=true;
						usleep(5000);
						if(recieved_data->Compass->calibrateDevice(true)==-1)
						{
							cout << "\033[0;45f Error writing to device" << endl;
							usleep(50000);
						}
						
						while(exitCal!='q')
						{
							exitCal=getchar();
						}
						recieved_data->Compass->exitCalibration();
						inputLock=false;
					break;
			case '9':	inputLock=true;
					recieved_data->Compass->restoreFactoryDefault();
					cout << "Factory Defaults Restored" << endl;
					usleep(500000);
					inputLock=false;
					break;	
			default:	break;
		}
	
	}


pthread_exit(NULL);

}


int main(void)
{
        int rc;
	pthread_t threads[2];
	struct thread_data td[2];

	GPIO gpio;		//GPIO controller object
	I2C  i2c;
	Orientation cmps11;

	gpio.m_toggleI2C();		//enable I2C Mode also prevents changes to I2C pins
	i2c.setSlaveAddr(0x2a);		//set the slave address of the device
	i2c.setBaud(80000); 		//set baud to 90Khz to prevent clock stretching.
	system("stty raw");
	
	cout << "\e[?25l";       	// HIDE THE CURSOR
	for(int i=0; i<2; i++)
	{
		td[i].threadid=i;
		td[i].Compass=&cmps11;
		td[i].i2cptr=&i2c;
		switch(i)
		{
		
			case 0:	rc=pthread_create(&threads[i],NULL,displayMenu,(void *)&td[i]);
				if(rc)
				{
					cout << "Cannot Creater Thread!" << endl;
				}
				break;
			case 1:	rc=pthread_create(&threads[i],NULL,displayBearing,(void *)&td[i]);
				if(rc)
				{
					cout << "Cannot Creater Thread!" << endl;
				}
				break;
		}
	}

	
	pthread_join(threads[0],NULL); //join the main menu thread
	
	//cleanup
	
	pthread_cancel(threads[1]); // cancel the bearing thread
	system("reset");
	pthread_exit(NULL); //call this last always otherwise it will skip all of the text before it.
return(0);
}
