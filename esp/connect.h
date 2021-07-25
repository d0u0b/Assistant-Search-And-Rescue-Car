#ifndef _CONNECT_H_
#define _CONNECT_H_

#include "Arduino.h"
#include "config.h"

#include <esp_task_wdt.h>
#define signalDelay(n) { int i; for (i = 0; i < n; ++i); }

void connectInitial(void);

void sendCommand(CMD cmd);
bool waitReceive(void);

void startConnect(void);
bool startReceive(void);
void endConnect(void);

uint8_t receiveData(CMD cmd);
void receiveData(CMD cmd, uint8_t *buffer, int length);

uint8_t receiveByte(void);
void receiveBytes(uint8_t *buffer, int len);

#endif
