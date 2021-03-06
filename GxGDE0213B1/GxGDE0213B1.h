/************************************************************************************
   class GxGDE0213B1 : Display class example for GDE0213B1 e-Paper from Dalian Good Display Co., Ltd.: www.good-display.com

   based on Demo Example from Good Display, available here: http://www.good-display.com/download_detail/downloadsId=515.html

   Author : J-M Zingg

   Version : 2.2

   Support: limited, provided as example, no claim to be fit for serious use

   Controller: IL3895 : http://www.good-display.com/download_detail/downloadsId=538.html

   connection to the e-Paper display is through DESTM32-S2 connection board, available from Good Display

   DESTM32-S2 pinout (top, component side view):
         |-------------------------------------------------
         |  VCC  |o o| VCC 5V  not needed
         |  GND  |o o| GND
         |  3.3  |o o| 3.3     3.3V
         |  nc   |o o| nc
         |  nc   |o o| nc
         |  nc   |o o| nc
   MOSI  |  DIN  |o o| CLK     SCK
   SS    |  CS   |o o| DC      e.g. D3
   D4    |  RST  |o o| BUSY    e.g. D2
         |  nc   |o o| BS      GND
         |-------------------------------------------------
*/
#ifndef _GxGDE0213B1_H_
#define _GxGDE0213B1_H_

#include "../GxEPD.h"

// the physical number of pixels (for controller parameter)
#define GxGDE0213B1_X_PIXELS 128
#define GxGDE0213B1_Y_PIXELS 250

// the logical width and height of the display
#define GxGDE0213B1_WIDTH GxGDE0213B1_X_PIXELS
#define GxGDE0213B1_HEIGHT GxGDE0213B1_Y_PIXELS

// note: the visible number of display pixels is 122*250, see GDEH0213B1 V3.0 Specification.pdf
#define GxGDE0213B1_VISIBLE_WIDTH 122

#define GxGDE0213B1_BUFFER_SIZE (uint32_t(GxGDE0213B1_WIDTH) * uint32_t(GxGDE0213B1_HEIGHT) / 8)

// divisor for AVR, should be factor of GxGDE0213B1_HEIGHT
#define GxGDE0213B1_PAGES 5

#define GxGDE0213B1_PAGE_HEIGHT (GxGDE0213B1_HEIGHT / GxGDE0213B1_PAGES)
#define GxGDE0213B1_PAGE_SIZE (GxGDE0213B1_BUFFER_SIZE / GxGDE0213B1_PAGES)

// mapping suggestion from Waveshare 2.13inch e-Paper to Wemos D1 mini
// BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V

// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

// mapping suggestion for AVR, UNO, NANO etc.
// BUSY -> 7, RST -> 9, DC -> 8, CS-> 10, CLK -> 13, DIN -> 11

class GxGDE0213B1 : public GxEPD
{
  public:
#if defined(ESP8266)
    //GxGDE0213B1(GxIO& io, uint8_t rst = D4, uint8_t busy = D2);
    // use pin numbers, other ESP8266 than Wemos may not use Dx names
    GxGDE0213B1(GxIO& io, uint8_t rst = 2, uint8_t busy = 4);
#else
    GxGDE0213B1(GxIO& io, uint8_t rst = 9, uint8_t busy = 7);
#endif
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void init(void);
    void fillScreen(uint16_t color); // 0x0 black, >0x0 white, to buffer
    void update(void);
    // to buffer, may be cropped, drawPixel() used, update needed
    void  drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode = bm_normal);
    // to full screen, filled with white if size is less, no update needed
    void drawBitmap(const uint8_t *bitmap, uint32_t size, int16_t mode = bm_normal); // only bm_normal, bm_invert, bm_partial_update modes implemented
    void eraseDisplay(bool using_partial_update = false);
    // partial update of rectangle from buffer to screen, does not power off
    void updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation = true);
    // partial update of rectangle at (xs,ys) from buffer to screen at (xd,yd), does not power off
    void updateToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation = true);
    // terminate cleanly updateWindow or updateToWindow before removing power or long delays
    void powerDown();
    // paged drawing, for limited RAM, drawCallback() is called GxGDE0213B1_PAGES times
    // each call of drawCallback() should draw the same
    void drawPaged(void (*drawCallback)(void));
    void drawPaged(void (*drawCallback)(uint32_t), uint32_t);
    void drawPaged(void (*drawCallback)(const void*), const void*);
    void drawPaged(void (*drawCallback)(const void*, const void*), const void*, const void*);
    // paged drawing to screen rectangle at (x,y) using partial update
    void drawPagedToWindow(void (*drawCallback)(void), uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void drawPagedToWindow(void (*drawCallback)(uint32_t), uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t);
    void drawPagedToWindow(void (*drawCallback)(const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void*);
    void drawPagedToWindow(void (*drawCallback)(const void*, const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void*, const void*);
    void drawCornerTest(uint8_t em = 0x01);
  private:
    void _writeToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h);
    void _writeData(uint8_t data);
    void _writeCommand(uint8_t command);
    void _writeCommandData(const uint8_t* pCommandData, uint8_t datalen);
    void _SetRamPointer(uint8_t addrX, uint8_t addrY, uint8_t addrY1);
    void _SetRamArea(uint8_t Xstart, uint8_t Xend, uint8_t Ystart, uint8_t Ystart1, uint8_t Yend, uint8_t Yend1);
    void _PowerOn(void);
    void _PowerOff(void);
    void _waitWhileBusy(const char* comment=0);
    void _setRamDataEntryMode(uint8_t em);
    void _InitDisplay(uint8_t em);
    void _Init_Full(uint8_t em);
    void _Init_Part(uint8_t em);
    void _Update_Full(void);
    void _Update_Part(void);
    void _rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h);
  protected:
#if defined(__AVR)
    uint8_t _buffer[GxGDE0213B1_PAGE_SIZE];
#else
    uint8_t _buffer[GxGDE0213B1_BUFFER_SIZE];
#endif
  private:
    GxIO& IO;
    int16_t _current_page;
    bool _using_partial_mode;
    uint8_t _rst;
    uint8_t _busy;
    static const uint8_t LUTDefault_full[];
    static const uint8_t LUTDefault_part[];
    static const uint8_t GDOControl[];
    static const uint8_t softstart[];
    static const uint8_t VCOMVol[];
    static const uint8_t DummyLine[];
    static const uint8_t Gatetime[];
#if defined(ESP8266) || defined(ESP32)
  public:
    // the compiler of these packages has a problem with signature matching to base classes
    void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
    {
      Adafruit_GFX::drawBitmap(x, y, bitmap, w, h, color);
    };
#endif
};

#define GxEPD_Class GxGDE0213B1

#define GxEPD_WIDTH GxGDE0213B1_WIDTH
#define GxEPD_HEIGHT GxGDE0213B1_HEIGHT
#define GxEPD_BitmapExamples <GxGDE0213B1/BitmapExamples.h>
#define GxEPD_BitmapExamplesQ "GxGDE0213B1/BitmapExamples.h"

#endif

