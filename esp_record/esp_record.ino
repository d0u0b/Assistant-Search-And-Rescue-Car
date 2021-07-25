#include "config.h"
#include "connect.h"
#include <WiFi.h>
#include <WiFiClient.h>

const char *ssid = "wiFiSSID";
const char *password = "WiFiPasswrod";

WiFiClient client;

void setup()
{
    Serial.begin(115200);
    connectInitial();
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

}

void loop()
{
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again
    client.connect("192.168.0.107", 5000);
    client.setNoDelay(true);
    startConnect();
    sendCommand(GET_DOA);
    if (startReceive())
    {
        endConnect();
        client.stop();
        Serial.println("GET_DOA Timeout!");
        return;
    }
    uint8_t img_buffer[1600];
    uint16_t i = 200;
    while (i--)
    {
        receiveBytes(img_buffer, 1600);
        client.write(img_buffer,1600);
    }
    endConnect();
    client.stop();
}
