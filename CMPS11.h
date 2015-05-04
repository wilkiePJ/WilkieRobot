// CMPS11 Driver class - 
// Author: Phil Wilkins 
// Create Date: April 24 2015 
// Version: 0.1
// Dependancies : BCM2835.h - clone from git @ https://github.com/wilkiePJ/BCM2835.git

#ifndef CMPS11_H
#define CMPS11_H

#include "BCM2835.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
// Register layout for the CMPS11 in I2C mode.

#define CMPS11_ADDR 0x60		//default value returned.

#define COMMAND_REG 0 			//read for software version, write to issue a command.
#define COMPASS_8 1 			//8 bit 0-255 for full circle
#define COMPASS_16_HIGH 2		//16 bit full 360 degrees from the compass, this is the HIGH byte
#define COMPASS_16_LOW 3		//16 bit full 360 degrees from the compass, this is the LOW byte
#define PITCH_ANGLE 4			// 8 bit pitch angle with kaman filter (prevents reaction to shaking)
#define ROLL_ANGLE 5			// 8 bit roll angle with kaman filter (prevents reaction to shaking)
#define MAGNETOMETER_X_RAW_HIGH 6	// high byte compass raw data for specific Axis
#define MAGNETOMETER_X_RAW_LOW 7	// high byte compass raw data for specific Axis
#define MAGNETOMETER_Y_RAW_HIGH 8	// high byte compass raw data for specific Axis
#define MAGNETOMETER_Y_RAW_LOW 9	// high byte compass raw data for specific Axis
#define MAGNETOMETER_Z_RAW_HIGH 10 	// high byte compass raw data for specific Axis
#define MAGNETOMETER_Z_RAW_LOW 11	// high byte compass raw data for specific Axis
#define ACCELEROMETER_X_RAW_HIGH 12	// high byte compass raw data for specific Axis	
#define ACCELEROMETER_X_RAW_LOW 13	// high byte compass raw data for specific Axis
#define ACCELEROMETER_Y_RAW_HIGH 14	// high byte compass raw data for specific Axis
#define ACCELEROMETER_Y_RAW_LOW 15	// high byte compass raw data for specific Axis
#define ACCELEROMETER_Z_RAW_HIGH 16	// high byte compass raw data for specific Axis
#define ACCELEROMETER_Z_RAW_LOW 17	// high byte compass raw data for specific Axis
#define GYRO_X_RAW_HIGH 18		// high byte compass raw data for specific Axis
#define GYRO_X_RAW_LOW 19		// high byte compass raw data for specific Axis
#define GYRO_Y_RAW_HIGH 20		// high byte compass raw data for specific Axis
#define GYRO_Y_RAW_LOW 21		// high byte compass raw data for specific Axis
#define GYRO_Z_RAW_HIGH 22		// high byte compass raw data for specific Axis
#define GYRO_Z_RAW_LOW 23		// high byte compass raw data for specific Axis
#define TEMP_RAW_HIGH 24		// high byte temperature data
#define TEMP_RAW_LOW 25			// low byte temperature data 
#define PITCH_ANGLE_NO_FILTER 26	// pitch angle - no filter 8 bit
#define ROLL_ANGLE_NO_FILTER 27		// roll angle - no filter 8 bit

struct m_coord
{
	short x,y,z;
};

class Orientation : public I2C
{
private:
	int8_t m_pitch;
	int8_t m_roll;	
	int8_t m_pitch_nofilt;
	int8_t m_roll_nofilt;
	float m_compassBearing;
	short m_Temperature;	
	struct m_coord m_magnometer, m_gyro, m_accelerometer;
	
	short convertShort(int8_t high, int8_t low);	
public :
	void showSoftVersion();
	int calibrateDevice();
	int restoreFactoryDefault();	
	float m_getBearing16(); 
	short m_getBearing8();
	struct m_coord getMagnometer();
	struct m_coord getGyro();
	struct m_coord getAccelerometer();
	int8_t getPitch(bool mode);		// true=filter on, false=filter off
	int8_t getRoll(bool mode);
	Orientation();
	~Orientation();
};

#endif
