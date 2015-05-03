//Test.cpp - test main to test the driver.

#include "BCM2835.h"
#include "CMPS11.h"
#include <string.h>

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
		std::cout << "Written " << bytes_written << " bytes:Command Sent." 
				<< std::endl;
		usleep(500000);
	}
return;
}


int main(void)
{
        int selection=0,speed=0;
	bool exitLoop=false;
	
	GPIO gpio;		//GPIO controller object
	I2C  i2c;

	gpio.m_toggleI2C();		//enable I2C Mode also prevents changes to I2C pins
	i2c.setSlaveAddr(0x2a);		//set the slave address of the device
	i2c.setBaud(90000); 		//set baud to 90Khz to prevent clock stretching.
	
	while(!exitLoop)
	{

		system("clear");

		std::cout << "WILKIEROBOT Controller v0.1" << std::endl;
		std::cout << "---------------1----------------" << std::endl;	
		std::cout << "GPIO and I2C configured - Testing GPIO Output:" << std::endl;
		std::cout << "Enabling GPIO I2C Pins on the PI." << std::endl;	
		std::cout << "Setting Slave Address of the Arduino:" << std::endl;	
		std::cout << "Baud Set to 90kHz to prevent stretching.." << std::endl;	

		std::cout << "Select Mode:" << std::endl << 
		     "(1) Set Speed" << std::endl <<
		     "(2) Move Forwards" << std::endl <<
		     "(3) Move Backwards" << std::endl <<
		     "(4) Move Left" << std::endl <<	
		     "(5) Move Right" << std::endl <<
		     "(6) Stop" << std::endl <<
		     "(7) Quit" << std::endl;
		std::cin >> selection;

		switch(selection)
		{

			case 1:		std::cout << "Enter speed from 0 - 255:";
					std::cin >> speed;
					break;
			case 2:		SendCommand(i2c,10 & 0xFF,speed);
					break;
			case 3:		SendCommand(i2c,20 & 0xFF,speed);
					break;
			case 4:		SendCommand(i2c,30 & 0xFF,speed);
					break;
			case 5:		SendCommand(i2c,40 & 0xFF,speed);
					break;
			case 6:		SendCommand(i2c,50 & 0xFF,speed);
					break;
			case 7:		exitLoop=true;
					break;
			default:	break;
		}
	
	}
	
	return(0);
}
