#include "motor.h"

//private
void Motor::setSpeed(uint8_t speed) {
    ledcWrite(0, min(230, speed + (motorFix * rightDirect)));
    ledcWrite(1, min(230, speed - (motorFix * leftDirect)));
    motorRight = min(230, speed + (motorFix * rightDirect));
    motorLeft = min(230, speed - (motorFix * leftDirect));
    currentSpeed = speed;
}

void Motor::setSpeed(uint8_t speedLeft, uint8_t speedRight) {
    ledcWrite(0, speedRight);
    ledcWrite(1, speedLeft);
    motorRight = speedRight;
    motorLeft = speedLeft;
}

void Motor::setSpeed() {
    ledcWrite(0, currentSpeed + (motorFix * rightDirect));
    ledcWrite(1, currentSpeed - (motorFix * leftDirect));
    motorRight = currentSpeed + (motorFix * rightDirect);
    motorLeft = currentSpeed - (motorFix * leftDirect);
}

void Motor::setMotorDirect(int8_t left, int8_t right) {
    leftDirect = left;
    rightDirect = rightDirect;
    digitalWrite(L1, left == 1);
    digitalWrite(L2, left == -1);

    digitalWrite(R1, right == 1);
    digitalWrite(R2, right == -1);
}

float Motor::degreeDiff(float a, float b) {
    float buffer = a - b;
    return buffer > 180 ? buffer - 360 : (buffer < -180 ? buffer + 360 : buffer);
}

//public
void Motor::initial() {
    pinMode(R1, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(L1, OUTPUT);
    pinMode(L2, OUTPUT);

    ledcSetup(0, pwd_freq, resolution);
    ledcAttachPin(RP, 0);
    ledcSetup(1, pwd_freq, resolution);
    ledcAttachPin(LP, 1);
}

void Motor::start() {
    Motor::forward();
}

void Motor::stop() {
    Motor::setMotorDirect(0, 0);
    state = STOP;
}

void Motor::forward() {
    if(state == FORWARD)return;
    targetYaw = *yaw;
    Motor::setSpeed(180);
    Motor::setMotorDirect(1, 1);
    state = FORWARD;
}

void Motor::back() {
    if(state == BACK)return;
    targetYaw = *yaw;
    Motor::setSpeed(180);
    Motor::setMotorDirect(-1, -1);
    state = BACK;
}

void Motor::goLeft() {
    if(state != GOLEFT) {
        Motor::forward();
        state = GOLEFT;
    }
    correctRotate(70);
}

void Motor::goRight() {
    if(state != GORIGHT) {
        Motor::forward();
        state = GORIGHT;
    }
    correctRotate(-70);
}

void Motor::turnLeft() {
    state = TURNLEFT;
    targetYaw = *yaw - 90;
    if(targetYaw < -180) targetYaw += 360;
    Motor::setSpeed(170, 170);
    Motor::setMotorDirect(-1, 1);
    while(abs(degreeDiff(targetYaw, *yaw)) > 5)vTaskDelay(1);
    Motor::forward();
}

void Motor::turnRight() {
    state = TURNRIGHT;
    targetYaw = *yaw + 90;
    if(targetYaw > 180) targetYaw -= 360;
    Motor::setSpeed(170, 170);
    Motor::setMotorDirect(1, -1);

    while(abs(degreeDiff(targetYaw, *yaw)) > 5)vTaskDelay(1);
    Motor::forward();
}

void Motor::correctYaw() {
    float diff = Motor::degreeDiff(targetYaw, *yaw);

    if(diff > 30) return correctRotate(30);
    if(diff > 5) return correctRotate(10);
    if(diff > 1) return correctRotate(1);
    if(diff > -1) return correctRotate(0);
    if(diff > -5) return correctRotate(-1);
    if(diff > -30) return correctRotate(-10);
    return correctRotate(-30);
}

void Motor::correctRotate(float rotateSpeed) {
    float rotateDiff = rotateSpeed - gyro[2];
    // Serial.println(gyro[2]);
    if(rotateDiff > 30) motorFix += 30;
    else if(rotateDiff > 10) motorFix += 15;
    else if(rotateDiff > 0.1) motorFix += 1;
    else if(rotateDiff > -0.1);
    else if(rotateDiff > -10) motorFix -= 1;
    else if(rotateDiff > -30) motorFix -= 15;
    else motorFix -= 30;

    motorFix = min(70, max(-70, (int)motorFix));
    if(abs(motorFix) > 0.1) Motor::setSpeed();
}