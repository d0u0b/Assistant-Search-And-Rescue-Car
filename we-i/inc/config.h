#ifndef _CONFIG_H_
#define _CONFIG_H_

typedef unsigned int uint;

//Connect Command
typedef enum __packed
{
	PERSON_DETECT = 0x81,
	GET_DOA = 0x84,
	GET_IMAGE = 0x93,
	None = 0x00
} CMD;

//Mic Config
// #define MIC_MAX_QUEUE_LENGTH 50

#endif
