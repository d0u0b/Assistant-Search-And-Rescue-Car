#define CLK 34
#define SS 35
#define MOSI 32

#define trigger1 33
#define echo1 39
#define trigger2 25
#define echo2 36

#define RP 26
#define R1 27
#define R2 17
#define L1 4
#define L2 16
#define LP 2

#include <WiFi.h>
#include <ArduinoQueue.h>
#include <esp_task_wdt.h>
#include <HCSR04.h>

const char *ssid = "******";
const char *password = "******";
const char *server = "***.***.***.***";
WiFiClient client;

HCSR04 hc1(trigger1, echo1);
HCSR04 hc2(trigger2, echo2);

bool clk = false;

TaskHandle_t spiTaskHandle;

typedef enum __attribute__((packed))
{
  SPI_IDLE,
  SPI_CMD,
  SPI_DEBUG = 0x81,
  SPI_IMG = 0x82
} SPI_STATE;

typedef enum __attribute__((packed))
{
  MAIN,
  IMG
} MODE;

MODE currentMode, lastMode = MAIN;
ArduinoQueue<uint8_t> SPIResvQueue(512);

uint8_t resv = 0, count = 8;
uint16_t dataBufferCount = 0;

void IRAM_ATTR SSIsr()
{
  resv = 0;
  count = 8;
}

void IRAM_ATTR CLKIsr()
{
  resv += digitalRead(MOSI);
  if (!--count)
  {
    SPIResvQueue.enqueue(resv);
    resv = 0;
    count = 8;
    return;
  }
  resv <<= 1;
}

uint32_t dataCount;
ArduinoQueue<String> sendBuffer(300);
String dataBuffer = "";

void IRAM_ATTR spiTask(void *pvParameters)
{
  for (;;)
  {
    vTaskDelay(1);
    while (!SPIResvQueue.isEmpty())
    {
      switch ((SPI_STATE)SPIResvQueue.dequeue())
      {
      case SPI_IMG:
        lastMode = IMG;
        // dataCount = 300;
        // while (dataCount--)
        // {
        //   vTaskDelay(1);
        //   while (dataBufferCount++ < 1024)
        //   {
        //     while (SPIResvQueue.isEmpty())
        //     {
        //       Serial.printf("%d %d\n", SPIResvQueue.itemCount(), dataCount);
        //       vTaskDelay(1);
        //     }
        //     dataBuffer += (char)SPIResvQueue.dequeue();
        //   }
        //   sendBuffer.enqueue(dataBuffer);
        //   dataBuffer = "";
        //   dataBufferCount = 0;
        // }

        // dataCount = 307200;
        // while (dataCount)
        // {
        //   while (SPIResvQueue.itemCount() == 0){
        //     vTaskDelay(1);
        //   }
          
        //   dataCount--;
        //   dataBuffer += (char)SPIResvQueue.dequeue();
        //   if (++dataBufferCount == 1024)
        //   {
        //     Serial.printf("%d %d\n", SPIResvQueue.itemCount(), dataCount);
        //     sendBuffer.enqueue(dataBuffer);
        //     dataBuffer = "";
        //     dataBufferCount = 0;
        //     vTaskDelay(1);
        //   }
          
        // }
        break;
      }
      //Serial.print((char)SPIResvQueue.dequeue());
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(SS, INPUT_PULLUP);
  pinMode(MOSI, INPUT_PULLUP);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  ledcSetup(0, pwd_freq, resolution);
  ledcAttachPin(RP, 0);
  ledcSetup(1, pwd_freq, resolution);
  ledcAttachPin(LP, 1);
  // GPIO.out_w1tc = ((uint32_t)1 << SCLK);
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());
  if (client.connect(server, 5000))
  {
    Serial.println("Server connected");
  }

  // xTaskCreatePinnedToCore(
  //     spiTask,        /*任務實際對應的Function*/
  //     "Task1",        /*任務名稱*/
  //     10000,          /*堆疊空間*/
  //     NULL,           /*無輸入值*/
  //     0,             /*優先序0*/
  //     &spiTaskHandle, /*對應的任務變數位址*/
  //     0               /*指定在核心0執行 */
  // );

  // attachInterrupt(SS, SSIsr, RISING);
  // attachInterrupt(CLK, CLKIsr, RISING);
}

void loop()
{
  if (currentMode != lastMode)
  {
    currentMode = lastMode;
    switch (currentMode)
    {
    case MAIN:
      break;
    case IMG:
      Serial.println("send img");
      client.print("img");
      break;
    }
  }
  switch (currentMode)
  {
  case MAIN:
    delay(50);
    Serial.printf("%f ", hc1.dist());
    delay(50);
    Serial.printf("%f\n", hc2.dist());
    break;
  case IMG:
    if (!sendBuffer.isEmpty())
    {
      client.print(sendBuffer.dequeue());
    }
    if (!dataCount && sendBuffer.isEmpty())
    {
      Serial.println("back to main");
      lastMode = MAIN;
    }
    break;
  }
}
