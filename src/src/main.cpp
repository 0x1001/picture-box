#include <Arduino.h>
// GxEPD_HD_TestExample : test example for HD e-Paper displays from Dalian Good Display Inc. (parallel interface).
//
// Created by Jean-Marc Zingg based on demo code from Good Display for red DESTM32-L board.
//
// To be used with "BLACK 407ZE (V3.0)" of "BLACK F407VE/ZE/ZG boards" of package "STM32GENERIC for STM32 boards" for Arduino.
// https://github.com/danieleff/STM32GENERIC
//
// The e-paper displays and demo board are available from:
//
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571&product_id=22833
// or https://www.aliexpress.com/store/product/Epaper-demo-kit-for-6-800X600-epaper-display-GDE060BA/600281_32812255729.html
//
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_8371&product_id=15441
// or https://www.aliexpress.com/store/product/GDE080A1-with-demo-8-epaper-display-panel-1024X768-with-demo/600281_32810750410.html

// include library, include base class, make path known
#include <Adafruit_GFX.h>
#include <GxEPD.h>

// select the display class to use, only one
//#include "GxGDE043A2/GxGDE043A2.cpp"
//#include "GxGDE060BA/GxGDE060BA.cpp"
#include "GxGDEW042T2/GxGDEW042T2.cpp"

// uncomment next line for drawBitmap() test
//#include GxEPD_BitmapExamples

#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

GxIO_Class io(SPI, /*CS=*/ 10, /*DC=*/ 8, /*RST=*/ 9);
GxEPD_Class display(io, /*RST=*/ 9, /*BUSY=*/ 7);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");

  display.init();

  Serial.println("setup done");
}

void showBitmapExample()
{
#ifdef _GxBitmapExamples_H_
  //display.drawPicture(BitmapExample1, sizeof(BitmapExample1));
  delay(5000);
  //display.erasePicture(BitmapExample2, sizeof(BitmapExample2));
  //display.setRotation(2);
  display.fillScreen(GxEPD_WHITE);
  //display.drawBitmap(bwBitmap640x384_1, (GxEPD_WIDTH - 640) / 2, (GxEPD_HEIGHT - 384) / 2, 640, 384, GxEPD_BLACK);
  display.update();
  delay(5000);
  display.fillScreen(GxEPD_WHITE);
  //display.drawBitmap(bwBitmap640x384_2, (GxEPD_WIDTH - 640) / 2, (GxEPD_HEIGHT - 384) / 2, 640, 384, GxEPD_BLACK, GxEPD::bm_flip_h);
  display.update();
  delay(10000);
#endif
}

void loop()
{
  showBitmapExample();
  delay(30000);
}
