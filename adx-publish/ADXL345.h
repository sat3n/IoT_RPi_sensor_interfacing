/*
 * ADXL345.h  Created on: 17 May 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring Raspberry Pi"
 * See: www.exploringrpi.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European
 * Union Public License (the "EUPL") version 1.1 as published by the
 * European Union. Any use of this Software, other than as authorized
 * under this License is strictly prohibited (to the extent such use
 * is covered by a right of the copyright holder of this Software).
 *
 * This Software is provided under the License on an "AS IS" basis and
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#ifndef ADXL345_H_
#define ADXL345_H_
#include"I2CDevice.h"

/// The ADXL345 has 0x40 registers (0x01 to 0x1C are reserved and should not be accessed)
#define BUFFER_SIZE 0x40

namespace exploringRPi {

/**
 * @class ADXL345
 * @brief Specific class for the ADXL345 Accelerometer that is a child of the I2CDevice class
 * Protected inheritance means that the public I2CDevice methods are not publicly accessible
 * by an object of the ADXL345 class.
 */
class ADXL345:protected I2CDevice{

public:

	/// An enumeration to define the gravity range of the sensor
	enum RANGE {
		PLUSMINUS_2_G 		= 0,//!< plus/minus 2g
		PLUSMINUS_4_G 		= 1,//!< plus/minus 4g
		PLUSMINUS_8_G 		= 2,//!< plus/minus 8g
		PLUSMINUS_16_G 		= 3 //!< plus/minus 16g
	};
	/// The resolution of the sensor. High is only available in +/- 16g range.
	enum RESOLUTION {
		NORMAL = 0,//!< NORMAL 10-bit resolution
		HIGH = 1   //!< HIGH 13-bit resolution
	};

private:
	unsigned int I2CBus, I2CAddress;
	unsigned char *registers;
	ADXL345::RANGE range;
	ADXL345::RESOLUTION resolution;
	short accelerationX, accelerationY, accelerationZ; // raw 2's complement values
	float pitch, roll;                                 // in degrees
	short combineRegisters(unsigned char msb, unsigned char lsb);
	void calculatePitchAndRoll();
	virtual int updateRegisters();

public:
	ADXL345(unsigned int I2CBus, unsigned int I2CAddress=0x53);
	virtual int readSensorState();

	virtual void setRange(ADXL345::RANGE range);
	virtual ADXL345::RANGE getRange() { return this->range; }
	virtual void setResolution(ADXL345::RESOLUTION resolution);
	virtual ADXL345::RESOLUTION getResolution() { return this->resolution; }

	virtual short getAccelerationX() { return accelerationX; }
	virtual short getAccelerationY() { return accelerationY; }
	virtual short getAccelerationZ() { return accelerationZ; }
	virtual float getPitch() { return pitch; }
	virtual float getRoll() { return roll; }

	// Debugging method to display and update the pitch/roll on the one line
	virtual void displayPitchAndRoll(int iterations = 6);
	virtual int publish(int iterations = 15);//publish method dec
	virtual ~ADXL345();
};

} /* namespace exploringRPi */

#endif /* ADXL345_H_ */
