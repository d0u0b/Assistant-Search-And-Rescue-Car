#ifndef _SERVER_H_
#define _SERVER_H_

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoQueue.h>
#include <esp_task_wdt.h>
#include "config.h"

typedef struct
{
    uint8_t* data;
    uint len;
} SendData;

class _Server {
    public:
        char* packet;
        void initial();
        void sendCMD(CMD cmd);
        void sendData(uint8_t* data, uint len);
        void sendEnqueue(CMD cmd, uint8_t* data, uint len);
        void flushQueue();
        uint8_t readCMD();


        _Server(){
            sendQueue = xQueueCreate(200, sizeof(SendData));
        };
        QueueHandle_t sendQueue = NULL;

    private:
        uint8_t counter;
        WiFiClient client;
        WiFiUDP Udp;
        const char* ssid = "chihuahua";
        const char* password = "11261224";
        IPAddress serverIP;
};

#endif