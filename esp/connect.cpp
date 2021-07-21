#include "connect.h"
#include <esp_task_wdt.h>

void connectInitial() {
    pinMode(SS, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(DATA, OUTPUT);
}

void startConnect() {
    digitalWrite(CLK, LOW);
    digitalWrite(SS, HIGH);
}

bool startReceive() {
    pinMode(DATA, INPUT_PULLUP);
    return waitReceive();
}

void endConnect() {
    digitalWrite(SS, LOW);
    pinMode(DATA, OUTPUT);
}

void sendCommand(CMD cmd) {
    uint8_t cmdByte = (uint8_t)cmd;
    for(int8_t i = 7; i >= 0; i--) {
        digitalWrite(DATA, cmdByte >> i & 1);
        signalDelay(35);
        digitalWrite(CLK, HIGH);
        signalDelay(35);
        digitalWrite(CLK, LOW);
    }
}

bool waitReceive() {
    uint16_t timeout = 0;
    digitalWrite(CLK, HIGH);
    while(--timeout){
        if(!digitalRead(DATA))return false;
        delayMicroseconds(10);
    }
    Serial.println("Timeout!");
    return true;
}

uint8_t receiveByte() {
    uint8_t buffer = 0;
    for(int8_t i = 0; i < 8; i++) {
        digitalWrite(CLK, LOW);
        signalDelay(35);
        digitalWrite(CLK, HIGH);
        buffer <<= 1;
        buffer += digitalRead(DATA);
        signalDelay(35);
    }
    digitalWrite(CLK, LOW);
    return buffer;
}

void receiveBytes(uint8_t* buffer, int len) {
    while(len--) {
        for(int8_t i = 0; i < 8; i++) {
            digitalWrite(CLK, LOW);
            signalDelay(35);
            digitalWrite(CLK, HIGH);
            *buffer <<= 1;
            *buffer += digitalRead(DATA);
            signalDelay(35);
        }
        buffer++;
    }
    digitalWrite(CLK, LOW);
}

uint8_t receiveData(CMD cmd) {
    startConnect();
    sendCommand(cmd);
    if(startReceive()){
        endConnect();
        return 0;
    }
    uint8_t buffer;
    buffer = receiveByte();
    endConnect();
    return buffer;
}

void receiveData(CMD cmd, uint8_t* buffer, int length) {
    startConnect();
    sendCommand(cmd);
    if(startReceive()){
        endConnect();
        return;
    }
    receiveBytes(buffer, length);
    endConnect();
}