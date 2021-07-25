#ifndef _SERVER_H_
#define _SERVER_H_

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoQueue.h>
#include <esp_task_wdt.h>
#include "config.h"

typedef struct {
	uint8_t *data;
	uint len;
} SendData;

class _Server
{
public:
	char *packet;
	QueueHandle_t sendQueue = NULL;

	void initial(void);
	void sendCMD(CMD cmd);
	void sendData(uint8_t *data, uint len);
	void sendEnqueue(CMD cmd, uint8_t *data, uint len);
	void flushQueue(void);
	uint8_t readCMD(void);

	_Server() {
		sendQueue = xQueueCreate(200, sizeof(SendData));
	};

private:
	uint8_t counter;
	WiFiClient client;
	WiFiUDP Udp;
	const char *ssid = "wiFiSSID";
	const char *password = "WiFiPasswrod";
	IPAddress serverIP;
};

#endif
