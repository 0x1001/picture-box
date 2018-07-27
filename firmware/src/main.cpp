#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "epd4in2.h"
#include "epdpaint.h"
#include "LowPower.h"

#define SD_CHIP_SELECT 6

// Has to be multiple of 50: 50, 100, 150... First 50 bytes is first vertical line, Second 50 bytes is second vertical line, ...
#define BUFFER_SIZE 300

// Screen dimensions
#define SCREEN_WIDTH 400
#define SCREEN_HIGHT 300

// Used for changing image. Each time it is pressed new image comes up.
#define BUTTON 5

// Indicates load time of new image.
#define LED 4

// Screen driver
Epd epd;

// Image buffer for loading from SD card to screen.
unsigned char image_buffer[BUFFER_SIZE];

// Reads part of file from SD card
uint32_t readFilePart(const char * fileName, uint8_t * imageBuffer, int16_t size, uint32_t offset)
{
  static bool newFile = true;
  static File myFile;

  if (newFile)
  {
    myFile = SD.open(fileName);
    newFile = false;
  }

  int16_t bytesRead = 0;
  int16_t currentReadBytes = 0;
  if (myFile) {
    while (bytesRead < size) {
       currentReadBytes = myFile.read(imageBuffer + bytesRead, size);
       if (currentReadBytes == -1)
       {
         Serial.println("Errore generale");
         myFile.close();
         newFile = true;
         return 1;
       }
       bytesRead += currentReadBytes;
    }
    if ((offset + size) == ((SCREEN_WIDTH/8)*SCREEN_HIGHT))
    {
      myFile.close();
      newFile = true;
    }
  } else {
    newFile = true;
    return 1;
  }
  return 0;
}

// Selects random file from SD card. All files should be placed directly on SD root
void randomFile(char * fileName)
{
  uint32_t idx = 0;
  uint32_t fileCount = 1024;

  while (1)
  {
    idx = random(1, fileCount); // Starts from one: pictures should be named 1.bin, 2.bin, 3.bin, ..., 100.bin, ..., 355.bin, ...
    sprintf(fileName, "%d.bin", (int)idx);
    if (SD.exists(fileName))
    {
        return;
    }
  }
}

void loadImage()
{
  static char fileName[10];
  Serial.println("SD init.");
  if (!SD.begin(SD_CHIP_SELECT)) {
     Serial.println("SD card initialization failed! Power cycle card.");
     return;
   }

  Serial.println("Chooseing random file...");
  randomFile(fileName);
  Serial.println("Done...");
  Serial.println("Choosen file");
  Serial.println(fileName);

  for (uint32_t i = 0; i < (((SCREEN_WIDTH/8)*SCREEN_HIGHT)/BUFFER_SIZE); i++)
  {
    readFilePart(fileName, image_buffer, BUFFER_SIZE, i*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, i*(BUFFER_SIZE / 50), SCREEN_WIDTH, BUFFER_SIZE / 50);
  }

  SD.end();
}

void setup() {
  Serial.begin(9600);
  delay(5);

  pinMode(SS, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.println("Picture box started");
  Serial.println("Screen init.");
  if (epd.Init() != 0) {
    Serial.println("e-Paper init failed");
    return;
  }

  randomSeed(analogRead(A0));
}

void loop() {
  if (digitalRead(BUTTON) == 0)
  {
    digitalWrite(LED, HIGH);
    loadImage();
    Serial.println("Screen refresh.");
    epd.DisplayFrame();
    digitalWrite(LED, LOW);
  } else
  {
    LowPower.powerDown(SLEEP_30MS, ADC_OFF, BOD_OFF);
  }
}
