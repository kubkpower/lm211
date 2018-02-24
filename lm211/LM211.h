#ifndef LM211_h
#define LM211_h

#include "Times_New_Roman__14.h"
#include "Times_New_Roman__6.h"
#include "Arduino.h"
#include "inttypes.h"
#include <assert.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 64

#define MAX_REFRESH_RATE 100

enum orientation {
  top,
  bottom,
  left,
  right
};

// Use STM32 PORTB
#define GLCD_RAW_PORT         GPIOB->regs->ODR
//#define GLCD_RAW_PORT         GPIOB->regs
#define GLCD_RAW_MODE_LOWER   GPIOB->regs->CRL
#define GLCD_RAW_MODE_HIGHER  GPIOB->regs->CRH

// control signals
#define GLCD_D1     10
#define GLCD_FLM    11 
#define GLCD_M      12
#define GLCD_CL1    13
#define GLCD_CL2    14
#define GLCD_D2     15

#define MIN_ANGLE 135


class LM211{
  public:
    // Inialize Pins
    LM211();
    
    //
    void clearScreen();
    
    // Redraw screen from buffer
    void refresh();

    void drawRectangle (int x, int y, int width, int height);
    void drawFilledRectangle (int x, int y, int width, int height, bool black);

    void plotLine(int x0, int y0, int x1, int y1);
    void plotCircle(int xm, int ym, int r);
    void plotEllipseRect(int x0, int y0, int x1, int y1);
    void plotQuadBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2);
    void plotLineWidth(int x0, int y0, int x1, int y1, float wd, uint8_t color);
    void printString(int x,int y, const char *in, const struct FONT_DEF *strcut1, bool color);
    void printStringRight(int x,int y, const char *in, const struct FONT_DEF *strcut1, bool color);

    void createFilledCircle(int cx, int cy, int radius, uint8_t color, int percWidth, int beginAngle, int endAngle, bool dir);
    void drawArcBand (int x, int y, double degree, double radiusMax, double radiusMin, uint8_t color);
    void setValue(int value);
    void setPixel(int x, int y);
    void clearPixel(int x, int y);
    void writePixel(int x, int y, uint8_t color);

    // Set screen orientation
    void setOrientation(orientation dir) { screenDirection = dir; }

    //
    void writeBuffer(short x, short y, short value);

  private:
    unsigned char bufferA[60][64];
    int m;
    unsigned long timeRef;
    int frameRate;
    void myn_delay();

    
    unsigned int cmd = 0;    

    orientation screenDirection = top; 

    //
    // Internal routines
    //
    
    // Take 2 uint8_ts of buffer and writes it to screen (2 half of the screen)
    void lowWriteuint8_t (unsigned char value1, unsigned char pin1, unsigned char value2, unsigned char pin2);
    
    // write 2 bits from a uint8_t (through the mask) to the screen (2 half of the screen)
    void lowWritePixel(unsigned char value1, unsigned char value2, int mask, unsigned char pin1, unsigned char pin2);

    
    
        
    
};
#endif
