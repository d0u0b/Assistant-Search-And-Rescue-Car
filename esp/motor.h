#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <Arduino.h>
#include "config.h"

class Motor
{
private:

    typedef enum __attribute__((packed))
    {
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
    void setSpeed();
    void setMotorDirect(int8_t left, int8_t right);
    float degreeDiff(float a, float b);
public:
    STATE state;
    Motor(float *yawInput, int16_t *gyroInput):
        motorFix(0), 
        targetYaw(0), 
        state(STOP) {
            yaw = yawInput;
            gyro = gyroInput;
        }
    void initial();
    void correctYaw();
    void correctRotate(float rotateSpeed);
    void forward();
    void back();
    void goLeft();
    void goRight();
    void turnLeft();
    void turnRight();
    void start();
    void stop();
};


#endif