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

#define COLORED     0
#define UNCOLORED   1

Sd2Card card;
SdVolume volume;
SdFile root;
File myFile;

#define SD_CHIP_SELECT 6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  delay(5);

  Serial.println("e-Paper start");

  pinMode(SS, OUTPUT);

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  while (!card.init(SPI_HALF_SPEED, SD_CHIP_SELECT)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
  }


  // print the type of card
  Serial.print("\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);


  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);

  if (!SD.begin(SD_CHIP_SELECT)) {
    Serial.println("initialization failed!");
    return;
  }

  myFile = SD.open("TEST.TXT");

  if (myFile) {
  while (myFile.available()) {
    Serial.write(myFile.read());
  }
  myFile.close();
}
// if the file isn't open, pop up an error:
else {
  Serial.println("error opening test.txt");
}

  Epd epd;
  if (epd.Init() != 0) {
    Serial.println("e-Paper init failed");
    return;
  }

  /* This clears the SRAM of the e-paper display */
  epd.ClearFrame();

  /**
    * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
    * In this case, a smaller image buffer is allocated and you have to
    * update a partial display several times.
    * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
    */

  unsigned char image[1400];
  Paint paint(image, 400, 28);    //width should be the multiple of 8

  //epd.ClearFrame();
  //epd.DisplayFrame();

  paint.Clear(UNCOLORED);
  for(int i=0; i < 400; i++)
  {
    for(int j=0; j < 28; j++)
    {
      paint.DrawAbsolutePixel(i, j, COLORED);
    }
  }

  epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  //epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  //epd.SetPartialWindow(paint.GetImage(), 0, 28, paint.GetWidth(), paint.GetHeight());
  //epd.SetPartialWindow(paint.GetImage(), 0, 2*28, paint.GetWidth(), paint.GetHeight());

  /*
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "e-Paper Demo", &Font24, COLORED);
  epd.SetPartialWindow(paint.GetImage(), 100, 40, paint.GetWidth(), paint.GetHeight());

  paint.Clear(COLORED);
  paint.DrawStringAt(100, 2, "Hello world", &Font24, UNCOLORED);
  epd.SetPartialWindow(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);
  paint.SetHeight(64);

  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 0, 40, 50, COLORED);
  paint.DrawLine(0, 0, 40, 50, COLORED);
  paint.DrawLine(40, 0, 0, 50, COLORED);
  epd.SetPartialWindow(paint.GetImage(), 72, 120, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawCircle(32, 32, 30, COLORED);
  epd.SetPartialWindow(paint.GetImage(), 200, 120, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  epd.SetPartialWindow(paint.GetImage(), 72, 200, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledCircle(32, 32, 30, COLORED);
  epd.SetPartialWindow(paint.GetImage(), 200, 200, paint.GetWidth(), paint.GetHeight());
  */

  /* This displays the data from the SRAM in e-Paper module */
  epd.DisplayFrame();

  /* This displays an image */
  //epd.DisplayFrame(IMAGE_BUTTERFLY);

  /* Deep sleep */
  epd.Sleep();
}

void loop() {
  // put your main code here, to run repeatedly:

}
