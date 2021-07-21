#include "server.h"

void _Server::initial()
{
  //Connect WiFi
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  //Search Server
  Serial.println("WiFi connected");
  Serial.print("BROADCAST: ");
  serverIP = (IPAddress)((uint32_t(WiFi.localIP()) & uint32_t(WiFi.subnetMask())) + ~uint32_t(WiFi.subnetMask()));
  Serial.println(serverIP);
  Udp.begin(SERVER_PORT);

  while (1)
  {
    Udp.beginPacket(serverIP, SERVER_PORT);
    Udp.write(0x91);
    Udp.endPacket();
    while (++counter)
    {
      if (Udp.parsePacket())
      {
        serverIP = Udp.remoteIP();
        Udp.flush();
        Serial.print("Server: ");
        Serial.println(serverIP);
        break;
      }
      vTaskDelay(1);
    }
    if (counter)
    {
      counter = 0;
      break;
    }
    Serial.println("Timeout!");
  }
  Udp.stop();
  client.connect(serverIP, SERVER_PORT);
  client.setNoDelay(true);
}

void _Server::sendCMD(CMD cmd)
{
  client.write((uint8_t)cmd);
}

// void _Server::sendData(CMD cmd, uint8_t data)
// {
//   Udp.beginPacket(serverIP, SERVER_PORT);
//   Udp.write((uint8_t)cmd);
//   Udp.write(data);
//   Udp.endPacket();
// }

void _Server::sendData(uint8_t* data, uint len)
{
  client.write(data, len);
}

void _Server::flushQueue() {
  SendData buffer;
  // while(uxQueueMessagesWaiting(sendEnqueue)) {
    
  // }
  while(xQueueReceive(sendQueue, &buffer, 0) == pdPASS) {
    sendData(buffer.data, buffer.len);
    free(buffer.data);
    vTaskDelay(1);
  }
}

void _Server::sendEnqueue(CMD cmd, uint8_t* data, uint len) {
  SendData buffer = {
    .data = (uint8_t*)malloc(len + 1),
    .len = len + 1
  };
  memcpy(buffer.data + 1, data, len);
  *buffer.data = (uint8_t)cmd;
  xQueueSend(sendQueue, (void *)&buffer, 0);
};

uint8_t _Server::readCMD() {
  if(!client.available()) return 0;
  uint8_t buffer;
  client.read(&buffer, 1);
  return buffer;
}