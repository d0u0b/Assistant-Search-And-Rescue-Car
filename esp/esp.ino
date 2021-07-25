#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Wire.h>
#include <esp_task_wdt.h>
#include <HCSR04.h>
#include "connect.h"

#include "config.h"
#include "server.h"
#include "motor.h"

//MPU6050 data
MPU6050 mpu;
uint8_t fifoBuffer[64]; // FIFO storage buffer
uint8_t devStatus, mpuIntStatus, packetSize;
Quaternion q;        // [w, x, y, z]
VectorInt16 aa;      // [x, y, z]
VectorInt16 aaReal;  // [x, y, z]
VectorInt16 aaWorld; // [x, y, z]
VectorFloat gravity; // [x, y, z]
int16_t gyro[3];     // [x, y, z]
float euler[3];      // [psi, theta, phi]
float ypr[3];        // [yaw, pitch, roll]

_Server server;
Motor motor(ypr, gyro);
HCSR04 hcLeft(triggerL, echoL);
HCSR04 hcRight(triggerR, echoR);
float rightDist, leftDist;

TaskHandle_t attitudeHandle, sendDataHandle;

void getAttitudeTask(void *pvParameters)
{
  uint8_t attitudeSendBuffer[28]; // 4(time) + 6(accelrate) + 12(ypr) + 6(gyroscope)
  while (1)
  {
    *(uint32_t *)attitudeSendBuffer = millis();
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    {
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetAccel(&aa, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
      mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      mpu.dmpGetGyro(gyro, fifoBuffer);
      ypr[0] = ypr[0] * 180 / M_PI;
      // Serial.println(gyro[2]);
      memcpy(attitudeSendBuffer + 4, &aaWorld.x, 6);
      memcpy(attitudeSendBuffer + 10, ypr, 12);
      memcpy(attitudeSendBuffer + 22, gyro, 6);
      server.sendEnqueue(GET_ATTITUDE, attitudeSendBuffer, 28);
    }
    server.flushQueue();
  }
}

// void sendDataTask(void *pvParameters) {
//   while(1) {
//     server.flushQueue();
//     vTaskDelay(1);
//   }
// }

void setup()
{
  Serial.begin(115200);

  server.initial();
  connectInitial();
  Wire.begin();

  mpu.initialize();
  //MPU6050 initialize
  // mpu.setFullScaleAccelRange(0);
  // mpu.setFullScaleGyroRange(0);
  // Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788);

  if (devStatus == 0)
  {
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();

    mpu.setDMPEnabled(true);
    // attachInterrupt(digitalPinToInterrupt(35), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    packetSize = mpu.dmpGetFIFOPacketSize();
  }
  else
  {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
    ESP.restart();
  }
  motor.initial();

  xTaskCreate(
      getAttitudeTask,
      "getAttitudeTask",
      10000,
      NULL,
      configMAX_PRIORITIES,
      &attitudeHandle);

  // xTaskCreate(
  //   sendDataTask,
  //   "sendDataTask",
  //   10000,
  //   NULL,
  //   10,
  //   &sendDataHandle);
}
uint8_t count;
// TickType_t motorControlTaskTick = xTaskGetTickCount();

STATE state = STANDBY;
uint8_t recvCMD;

void loop()
{
  recvCMD = server.readCMD();
  switch (recvCMD)
  {
  case (uint8_t)CONTINUE_FIND:
    state = FINDING;
    return;
    break;
  case (uint8_t)GET_IMG:
    if (state == STANDBY)
    {
      startConnect();
      sendCommand(GET_IMG);
      if (startReceive())
      {
        endConnect();
        Serial.println("GET_IMG Timeout!");
        break;
      }
      vTaskSuspend(attitudeHandle);
      uint8_t img_buffer[1920];
      uint16_t i = 160;
      while (i--)
      {
        receiveBytes(img_buffer, 1920);
        server.sendEnqueue(GET_IMG, img_buffer, 1920);
        server.flushQueue();
      }
      endConnect();
      vTaskResume(attitudeHandle);
    }
    break;
  case (uint8_t)STOP_FIND:
    motor.stop();
    state = STANDBY;
    break;
  case (uint8_t)RESTART:
    ESP.restart();
    break;
  }

  switch (state)
  {
  case FINDING:
    if (receiveData(PERSON_DETECT))
    {
      motor.stop();
      state = STANDBY;
      server.sendCMD(PERSON_FINDED);
      break;
    }
    leftDist = hcLeft.dist();
    rightDist = hcRight.dist();
    server.sendEnqueue(GET_ULTRASONIC, (uint8_t*)&leftDist, 8);

    if (count)
    {
      motor.correctYaw();
      if (!--count)
      {
        motor.turnRight();
        // motor.forward();
      }
    }
    else
    {
      if (leftDist < 10 || rightDist < 10)
      {
        motor.back();
        count = 10;
      }
      else if (leftDist < 40)
        motor.goRight();
      else if (rightDist < 40)
        motor.goLeft();
      else
      {
        motor.forward();
        motor.correctYaw();
      }
      server.sendEnqueue(GET_MOTOR, (uint8_t*)&(motor.motorLeft), 2);
    }
    break;
  }
  // vTaskDelay(1);
  // server.flushQueue();
}