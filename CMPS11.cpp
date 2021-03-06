// CMPS11.cpp - Controller functions for the compass module
// Author: Phil Wilkins
// Create Date: 24/04/2015
// Version: 0.1
// Dependancies : BCM2835.h - clone from git @ https://github.com/wilkiePJ/BCM2835.git

#include <iostream>
#include "CMPS11.h"
#include <vector>


//FUNCTION showSoftVersion
//PURPOSE show the current software version of the hardware.
void Orientation::showSoftVersion()
{
	int softVersion=0;
	
	softVersion=readReg8(COMMAND_REG);
	if(softVersion==-1)
	{
		std::cout << "CMPS11 ERROR: Could not retrieve the software version" << std::endl;
		return;
	}
	else
	{
		std::cout << "CMPS11 Orientation Module Software version: " << softVersion << std::endl;
		return;
	}

}

//FUNCTION calibrateDevice
//PURPOSE Enter calibration mode true=horizontal mode only, false full with tilt compensation
int Orientation::calibrateDevice(bool mode)
{
	int err=0;
	std::vector<int> Buffer;
	
	if(mode)
 	{ 
		usleep(20000);
		Buffer.push_back(COMMAND_REG);
		Buffer.push_back(0xF0);
		err=writeData(Buffer,Buffer.size());
		Buffer.clear();
		usleep(20000);
		Buffer.push_back(COMMAND_REG);
		Buffer.push_back(0xF5);
		err=writeData(Buffer,Buffer.size());
		Buffer.clear();
		usleep(20000);	
		Buffer.push_back(COMMAND_REG);
		Buffer.push_back(0xF7);
		err=writeData(Buffer,Buffer.size());
		Buffer.clear();
		usleep(20000);
	}
 	
	else
	{
		usleep(20000);
		Buffer.push_back(COMMAND_REG);
		Buffer.push_back(0xF0);
		err=writeData(Buffer,Buffer.size());
		Buffer.clear();
		usleep(20000);
		Buffer.push_back(COMMAND_REG);
		Buffer.push_back(0xF5);
		err=writeData(Buffer,Buffer.size());
		Buffer.clear();
		usleep(20000);	
		Buffer.push_back(COMMAND_REG);
		Buffer.push_back(0xF6);
		err=writeData(Buffer,Buffer.size());
		Buffer.clear();
		usleep(20000);	

	}
 

return(err);
}

//FUNCTION: restoreFactoryDefault
//PURPOSE reset the device to factory default settings. Returns -1 on write error
int Orientation::restoreFactoryDefault()
{
	std::vector<int> Buffer;
	int err=0;

	usleep(20000);
	Buffer.push_back(COMMAND_REG);
	Buffer.push_back(0x20);
	err=writeData(Buffer,Buffer.size());
	Buffer.clear();
						
	usleep(20000);
	Buffer.push_back(COMMAND_REG);
	Buffer.push_back(0x2A);
	err=writeData(Buffer,Buffer.size());
	Buffer.clear();
			
	usleep(20000);	
	Buffer.push_back(COMMAND_REG);
	Buffer.push_back(0x60);
	err=writeData(Buffer,Buffer.size());
	Buffer.clear();			
	
	usleep(20000);

	return(err);
		
}

//FUNCTION: exitCalibration - exits calibration
//PURPOSE: stop the claibration mode and return to the normal mode of operation
int Orientation::exitCalibration()
{
	int err=0;
	std::vector<int> Buffer;

	Buffer.push_back(COMMAND_REG);
	Buffer.push_back(0xF8);
	err=writeData(Buffer,Buffer.size());
	Buffer.clear();
	usleep(20000);
	if(err==-1) return(-1);
	else
	{
		return(0);
	}
}

	
//FUNCTION: m_ getBearing16 
//PURPOSE read the compass data, returns a short. returns -1 on error, otherwise a compass bearing between 0 and 360 degrees
float Orientation::m_getBearing16()
{
	int err[2]={0,0};
	int8_t high=0;
	short result=0;
	

	//read compass angle 
	for(int i=0; i<2; i++)
	{
		err[i]=readReg8(i+COMPASS_16_HIGH);
		if(err[i]==-1)
		{
			result=-1;
		}
		else
		{
			if(i==0) high=(err[i] & 0xFF); else result=(short)((high <<8) | (err[i] & 0xFF)); 
		}
		usleep(1000);
	}
	
	return((float)(result)/10.0f);
	
}
 
//FUNCTION: m_getBearing8 
//PURPOSE read the compass data, returns a short. -1 on fail otherwise from 0 - 255 representing a full circle
short Orientation::m_getBearing8()
{
	int err=0;

	err=readReg8(COMPASS_8);
	if(err==-1) return(-1); else return(err);	
}


//FUNCTION getMagnetometer
//PURPOSE read the Magnetometer data, returns a structure - returns -1 on each coordinate if failes to read.
struct m_coord Orientation::getMagnometer()
{
	int err[6]={0,0,0,0,0,0};
	int8_t high=0;
	struct m_coord result;
	

	//read coordinates
	for(int i=0; i<6; i++)
	{
		err[i]=readReg8(i+MAGNETOMETER_X_RAW_HIGH);
		if(err[i]==-1)
		{
			result.x=-1;
			result.y=-1;
			result.z=-1;
		}
		else
		{
			if(i==0) high=(err[i] & 0xFF); else result.x=(short)((high <<8) | (err[i] & 0xFF)); 
			if(i==2) high=(err[i] & 0xFF); else result.y=(short)((high <<8) | (err[i] & 0xFF));
			if(i==4) high=(err[i] & 0xFF); else result.z=(short)((high <<8) | (err[i] & 0xFF)); 
		}
		usleep(1000);
	}
	
	return(result);
}


//FUNCTION getGyro
//PURPOSE read the Gyro data, returns a structure - returns -1 on each coordinate if failes to read.
struct m_coord Orientation::getGyro()
{
	int err[6]={0,0,0,0,0,0};
	int8_t high=0;
	struct m_coord result;
	

	//read coordinates
	for(int i=0; i<6; i++)
	{
		err[i]=readReg8(i+GYRO_X_RAW_HIGH);
		if(err[i]==-1)
		{
			result.x=-1;
			result.y=-1;
			result.z=-1;
		}
		else
		{
			if(i==0) high=(err[i] & 0xFF); else result.x=(short)((high <<8) | (err[i] & 0xFF)); 
			if(i==2) high=(err[i] & 0xFF); else result.y=(short)((high <<8) | (err[i] & 0xFF));
			if(i==4) high=(err[i] & 0xFF); else result.z=(short)((high <<8) | (err[i] & 0xFF)); 
		}
		usleep(1000);
	}
	
	return(result);
}

//FUNCTION getAccelerometer
//PURPOSE read the acceleromter data, returns a structure - returns -1 on each coordinate if failes to read.
struct m_coord Orientation::getAccelerometer()
{
	int err[6]={0,0,0,0,0,0};
	int8_t high=0;
	struct m_coord result;


	//read coordinates
	for(int i=0; i<6; i++)
	{
		err[i]=readReg8(i+ACCELEROMETER_X_RAW_HIGH);
		if(err[i]==-1)
		{
			result.x=-1;
			result.y=-1;
			result.z=-1;
		}
		else
		{
			if(i==0) high=(err[i] & 0xFF); else result.x=(short)((high <<8) | (err[i] & 0xFF)); 
			if(i==2) high=(err[i] & 0xFF); else result.y=(short)((high <<8) | (err[i] & 0xFF));
			if(i==4) high=(err[i] & 0xFF); else result.z=(short)((high <<8) | (err[i] & 0xFF)); 
		}
		usleep(1000);
	}
	
	return(result);
}

int8_t Orientation::getPitch(bool mode)
{		// true=filter on, false=filter off
	int err=0;

	switch(mode)
	{
		case true: break;
		case false: break;
		default: break;
	}
	return(0);
}

int8_t Orientation::getRoll(bool mode)
{
	int err=0;

	switch(mode)
	{
		case true: break;
		case false: break;
		default: break;
	}
	return(0);
}

short Orientation::convertShort(int8_t high, int8_t low)
{
	return((high << 8) | low);
}

//FUNCTION: Read temperature from the sensor, returns a signed int
int Orientation::m_getTemperature()
{
	int err[2]={0,0};
	int8_t high=0;
	int result=0;
	

	//read temperature 
	for(int i=0; i<2; i++)
	{
		err[i]=readReg8(i+TEMP_RAW_HIGH);
		if(err[i]==-1)
		{
			result=-1;
		}
		else
		{
			if(i==0) high=(err[i] & 0xFF); else result=(int)((high <<8) | (err[i] & 0xFF)); 
		}
		usleep(1000);
	}
	
	return(result);

}


Orientation::Orientation()
{
	//setBaud(100000);
	setSlaveAddr(CMPS11_ADDR); //set up the I2C address for the module
	configureDevice(I2C_BASE_ADDR);
}

Orientation::~Orientation()
{
	closeDevice();
}

