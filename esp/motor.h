#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <Arduino.h>
#include "config.h"

class Motor
{
private:
	typedef enum __packed {
		FORWARD,
		BACK,
		GOLEFT,
		GORIGHT,
		TURNLEFT,
		TURNRIGHT,
		STOP
	} STATE;
	float *yaw;
	int16_t *gyro;
	int8_t motorFix;
	uint8_t currentSpeed;
	float targetYaw;
	int8_t leftDirect, rightDirect;

	void setSpeed(uint8_t speedLeft, uint8_t speedRight);
	void setSpeed(uint8_t speed);
	void setSpeed(void);
	void setMotorDirect(int8_t left, int8_t right);
	float degreeDiff(float a, float b);

public:
	STATE state;
	uint8_t motorLeft, motorRight;

	Motor(float *yawInput, int16_t *gyroInput) : motorFix(0),
																							 targetYaw(0),
																							 state(STOP)
	{
		yaw = yawInput;
		gyro = gyroInput;
	}
	void initial(void);
	void correctYaw(void);
	void correctRotate(float rotateSpeed);
	void forward(void);
	void back(void);
	void goLeft(void);
	void goRight(void);
	void turnLeft(void);
	void turnRight(void);
	void start(void);
	void stop(void);
};

#endif
