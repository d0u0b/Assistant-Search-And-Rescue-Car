#ifndef _CONFIG_H_
#define _CONFIG_H_

//GPIO pins for WE-I Plus connection.
#define CLK 25
#define SS 26
#define DATA 27

//GPIO pins for HC-SR04 connection.
#define triggerR 33
#define echoR 36
#define triggerL 32
#define echoL 39

//GPIO pins for motor
#define RP 14
#define R1 13
#define R2 17
#define L1 4
#define L2 16
#define LP 2

#define pwd_freq 5000
#define resolution 8

//Config for server connection.
#define SERVER_PORT 5000

//MPU6050 pin
#define MPU6050_INT 35

typedef enum __packed {
	PERSON_DETECT = 0x81,
	GET_ATTITUDE = 0x82,
	GET_ULTRASONIC = 0x83,
	GET_DOA = 0x84,
	GET_MOTOR = 0x85,
	GET_IP = 0x91,
	PERSON_FINDED = 0x92,
	GET_IMG = 0x93,
	CONTINUE_FIND = 0xA1,
	STOP_FIND = 0xA2,
	RESTART = 0xA3
} CMD;

//esp32 state
typedef enum __packed {
	FINDING,
	STANDBY,
	TEST
} STATE;

//MPU6050 config
#define MPU6050_PATH 0x68
#define ValCnt 7
#define CalibrationCnt 200

#define DETECT_INTERVAL 15

#endif
