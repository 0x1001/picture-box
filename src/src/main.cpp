/**
 *  @filename   :   epd4in2-demo.ino
 *  @brief      :   4.2inch e-paper display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 4 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "epd4in2.h"
#include "epdpaint.h"

#define SD_CHIP_SELECT 6
#define BUFFER_SIZE 200
#define SCREEN_WIDTH 400
#define SCREEN_HIGHT 300

Sd2Card card;
SdVolume volume;
SdFile root;

int readFilePart(const char * fileName, uint8_t * imageBuffer, int16_t size, uint32_t offset)
{
  File myFile = SD.open(fileName);
  int16_t bytesRead = 0;
  int16_t currentReadBytes = 0;
  if (myFile) {
    myFile.seek(offset);

    while (bytesRead < size) {
       currentReadBytes = myFile.read(imageBuffer + bytesRead, size);
       if (currentReadBytes == -1)
       {
         Serial.println("Errore generale");
         myFile.close();
         return 1;
       }
       bytesRead += currentReadBytes;
    }
    myFile.close();

  } else {
    return 1;
  }
  return 0;
}

void setup() {
  static unsigned char image_buffer[BUFFER_SIZE];
  Epd epd;
  Serial.begin(9600);
  delay(5);

  Serial.println("Picture box started");
  Serial.println("Screen init.");
  if (epd.Init() != 0) {
    Serial.println("e-Paper init failed");
    return;
  }

  Serial.println("SD init.");
  pinMode(SS, OUTPUT);
  if (!SD.begin(SD_CHIP_SELECT)) {
     Serial.println("SD card initialization failed! Power cycle card.");
     return;
   }

  if (SD.exists("bd1.bin")) {
    Serial.println("file exists.");
  } else
  {
    Serial.println("file doesn't exist.");
    return;
  }

  Serial.println("Reading the file.");

  epd.ClearFrame();

  int heightCounter = 0;
  for (int i = 0; i < (((SCREEN_WIDTH/8)*SCREEN_HIGHT)/BUFFER_SIZE); i++)
  {

    /*
    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, 0*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, 0, 400, 4);

    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, 1*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, 4, 400, 4);

    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, 2*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, 8, 400, 4);

    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, 3*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, 12, 400, 4);

    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, 4*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, 16, 400, 4);

    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, 5*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, 20, 400, 4);

    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, 6*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, 24, 400, 4);

    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, 7*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, 28, 400, 4);

    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, 8*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, 32, 400, 4);

    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, 9*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, 36, 400, 4);

    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, 10*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, 40, 400, 4);

    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, 11*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, 44, 400, 4);

    break;
    */

    readFilePart("bd1.bin", image_buffer, BUFFER_SIZE, i*BUFFER_SIZE);
    epd.SetPartialWindow(image_buffer, 0, i*4, 400, 4);
  }

  Serial.println("Screen refresh.");
  epd.DisplayFrame();
}

void loop() {
}
