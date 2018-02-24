#include "LM211.h"

/*
 * Routines to reverse bits in a uint8_t
 */
static unsigned char lookup[16] = {
0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf, };

uint8_t reverse(uint8_t n) {
   // Reverse the top and bottom nibble then swap them.
   return (lookup[n&0b1111] << 4) | lookup[n>>4];
}

/* End Routines */


// Init
LM211::LM211(void) {
  m = 1;
  timeRef = 0;
  frameRate = 0;

  
  // Set port B as output
  GLCD_RAW_MODE_LOWER = 0x33333333;
  GLCD_RAW_MODE_HIGHER = 0x33333333;

  // Starts at HIGH
  GLCD_RAW_PORT |= ((1 << GLCD_CL1));
  
  // Starts at LOW
  GLCD_RAW_PORT &= ~((1 << GLCD_CL2)| (1 << GLCD_M) | (1 << GLCD_FLM) | (1 << GLCD_D1) | (1 << GLCD_D2));

  refresh();
  clearScreen();
  setOrientation(top);
  printString(0,5, "Initializing ..." ,&Times_New_Roman__6, 1);
  refresh();
}

// Try optimizing port toggle
void LM211::refresh(){
  short x;
  short y; 
  /*
  //Serial.println(timeRef);
  if ((timeRef + ((1/MAX_REFRESH_RATE)*1000)) > millis() ) {
    return;
    
  }
  timeRef = millis();
  */
  //frameRate += 1;
 
  // Start Line (Loop x64)
  cmd = 0;
  for (y=0; y<64; y++) {
      // CL1 DOWN
      GLCD_RAW_PORT &= ~(1 << GLCD_CL1);
      //cmd &= ~(1 << GLCD_CL1);
      
      if (y==1) {
          // SET FLM DOWN (no delay after CL1 DOWN) AND
          cmd &= ~(1 << GLCD_FLM);
          // SET M = !M
          if (m == 0) {
            m = 1;
            cmd |= (1 << GLCD_M);
          } else {
            m = 0;
            cmd &= ~(1 << GLCD_M);
          }
      }
      
      // Set Pixel (Loop 240x : 30 x 8bit)
      //lowWriteLine(y);
      x=0;
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b10000000, GLCD_D1, GLCD_D2);
      if (y == 0)
        cmd |= (1 << GLCD_FLM); // Placement ?
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b01000000, GLCD_D1, GLCD_D2);
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00100000, GLCD_D1, GLCD_D2);
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00010000, GLCD_D1, GLCD_D2);
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00001000, GLCD_D1, GLCD_D2);
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00000100, GLCD_D1, GLCD_D2);
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00000010, GLCD_D1, GLCD_D2);
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00000001, GLCD_D1, GLCD_D2);
      
      
      for (x=1; x<29; x++) {
        lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b10000000, GLCD_D1, GLCD_D2);
        lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b01000000, GLCD_D1, GLCD_D2);
        lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00100000, GLCD_D1, GLCD_D2);
        lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00010000, GLCD_D1, GLCD_D2);
        lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00001000, GLCD_D1, GLCD_D2);
        lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00000100, GLCD_D1, GLCD_D2);
        lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00000010, GLCD_D1, GLCD_D2);
        lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00000001, GLCD_D1, GLCD_D2);
        asm("nop");
      }
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b10000000, GLCD_D1, GLCD_D2);
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b01000000, GLCD_D1, GLCD_D2);
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00100000, GLCD_D1, GLCD_D2);
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00010000, GLCD_D1, GLCD_D2);
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00001000, GLCD_D1, GLCD_D2);
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00000100, GLCD_D1, GLCD_D2);
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00000010, GLCD_D1, GLCD_D2);
      
      lowWritePixel(bufferA[x][y], bufferA[x+30][y], 0b00000001, GLCD_D1, GLCD_D2);
      // IF last pixel of line : SET CL1 UP
      GLCD_RAW_PORT |= (1 << GLCD_CL1);
      
        
        
  }
  GLCD_RAW_PORT |= (1 << GLCD_D2);
  /*
  for (int z=y; z<(SCREEN_HEIGHT+1); z++) {
      // CL1 DOWN
      GLCD_RAW_PORT &= ~(1 << GLCD_CL1);
      for (x=0; x<479; x++) {
       // CL2 UP to use data
        GLCD_RAW_PORT |= (1 << GLCD_CL2);
        asm("nop");
        asm("nop");
        // CL2 DOWN to prepare next data
        GLCD_RAW_PORT &= ~(1 << GLCD_CL2);
      }
      // CL2 UP to use data
      GLCD_RAW_PORT |= (1 << GLCD_CL2);
      GLCD_RAW_PORT |= (1 << GLCD_CL1);
      asm("nop");
      // CL2 DOWN to prepare next data
      GLCD_RAW_PORT &= ~(1 << GLCD_CL2);
  }*/
}

void LM211::lowWriteuint8_t (unsigned char value1, unsigned char pin1, unsigned char value2, unsigned char pin2) {
  lowWritePixel(value1, value2, 0b10000000, pin1, pin2);
  lowWritePixel(value1, value2, 0b01000000, pin1, pin2);
  lowWritePixel(value1, value2, 0b00100000, pin1, pin2);
  lowWritePixel(value1, value2, 0b00010000, pin1, pin2);
  lowWritePixel(value1, value2, 0b00001000, pin1, pin2);
  lowWritePixel(value1, value2, 0b00000100, pin1, pin2);
  lowWritePixel(value1, value2, 0b00000010, pin1, pin2);
  lowWritePixel(value1, value2, 0b00000001, pin1, pin2);
}
void LM211::lowWritePixel(unsigned char value1, unsigned char value2, int mask, unsigned char pin1, unsigned char pin2) {
  if ((value1 & mask) == 0) {
    //bitClear(cmd, pin1);
    cmd &= ~(1 << pin1);
  } else {
    //bitSet(cmd, pin1);
    cmd |= (1 << pin1);
  }
  if ((value2 & mask) == 0) {
    //bitClear(cmd, pin2);
    cmd &= ~(1 << pin2);
  } else {
    //bitSet(cmd, pin2);
    cmd |= (1 << pin2);
  }
  
  // CL2 UP to use data
  GLCD_RAW_PORT = cmd;
  GLCD_RAW_PORT |= (1 << GLCD_CL2);
  
  // CL2 DOWN to prepare next data
  GLCD_RAW_PORT &= ~(1 << GLCD_CL2);
}


void LM211::clearScreen() {
  short a;
  short b;
  for (a=0; a<30; a++) {
    for (b=0; b<64; b++) {
      bufferA[a][b] = 0;
      bufferA[a+30][b] = 0;
    }
  }
}


void LM211::printString(int x,int y, const char *in, const struct FONT_DEF *strcut1, bool color)
{
  unsigned int offset;
  unsigned char width, by=0, mask=0;
  unsigned char Nruint8_ts;
  unsigned char i,j,map,height,allwidth=0;

  while((map = *in++)) 
  {
    map = pgm_read_byte(&strcut1->mapping_table[map]);

    width = strcut1->glyph_width;
    if(width == 0)
      width = pgm_read_byte(&strcut1->width_table[map]);

    offset = pgm_read_word(&strcut1->offset_table[map]);
    height = strcut1->glyph_height;

        Nruint8_ts = ((width-1)/8)+1;
  
    for(j=0; j<height * Nruint8_ts; j+=Nruint8_ts    )
    {   // height
      
      for(i=0 ; i<width  ; i++)
      {   //  width
          if(i%8 == 0)
          {
           by = pgm_read_byte(&strcut1->glyph_table[ offset+j+(i/8) ]);
           mask = 0x80;
          }
          
        
         
        if (color) {
          writePixel(x+i+allwidth , y+j/ Nruint8_ts, by & mask);
        } else {
          writePixel(x+i+allwidth , y+j/ Nruint8_ts, 0);
        }
        
        
        /*if( by & mask )
          setPixel(  x+i+allwidth , y+j/ Nruint8_ts);
        else
          clearPixel(  x+i+allwidth , y+j/ Nruint8_ts);
        */
        mask >>= 1;
      }//End i
    }// End j
    allwidth+=width;
  }// End K
} 

void LM211::printStringRight(int x,int my, const char *in, const struct FONT_DEF *strcut1, bool color)
{
  unsigned char width;
  unsigned char map,allwidth=0;
  const char *ref = in;

  while((map = *in++)) 
  {
    map = pgm_read_byte(&strcut1->mapping_table[map]);

    width = strcut1->glyph_width;
    if(width == 0)
      width = pgm_read_byte(&strcut1->width_table[map]);

    
    allwidth+=width;
  }// End K

  printString(x-allwidth, my, ref, strcut1, color);
} 


void LM211::writeBuffer(short x, short y, short value) {
  int offsetX = x; // Count uint8_t offset (buffer stores X as uint8_ts Array)
  int deltaX = x % 8; // count blank bits inside buffer X uint8_t
 
  // computed values for rotation
  int xp=SCREEN_WIDTH - 1 - y;
  int yp=x*8;
  uint8_t revValue = reverse(value);
  
  switch (screenDirection) {
    case top:
      if ((x>=0) && (x<(SCREEN_WIDTH/8)) && (y>=0) && (y<SCREEN_HEIGHT)) {
        bufferA[x][y] |= value;
      }
      break;
    case bottom:
      // LANDSCAPE REVERSE
      if ((x>=0) && (x<(SCREEN_WIDTH/8)) && (y>=0) && (y<SCREEN_HEIGHT)) {
        // il faut inverser l'odre des bits de la value
        bufferA[(SCREEN_WIDTH/8)-x-1][SCREEN_HEIGHT-y-1] |= revValue;
      }
      break;
    case right:
      offsetX = xp / 8;
      deltaX = xp % 8;
      if ((offsetX>=0) && (offsetX<(SCREEN_WIDTH/8)) && (yp>=0) && ((yp+7)<SCREEN_HEIGHT)) {  
        for (short n=0; n<8; n++) {
          if ((value & (128>>n)) != 0)
              bufferA[offsetX][yp + n] |= 128>>deltaX;
        }
      }
      break;
    case left:
      // PORTRAIT REVERSE
      offsetX = xp / 8;
      deltaX = xp % 8;
      if ((offsetX>=0) && (offsetX<(SCREEN_WIDTH/8)) && (yp>=0) && ((yp+7)<SCREEN_HEIGHT)) {  
        for (short n=0; n<8; n++) {
          if ((revValue & (1<<n)) != 0)
            bufferA[(SCREEN_WIDTH/8)-offsetX-1][SCREEN_HEIGHT-yp-1-n] |= (1 << deltaX);
        }
      }
      break;
  }
  // LANDSCAPE
  /*
  if ((x>=0) && (x<(SCREEN_WIDTH/8)) && (y>=0) && (y<SCREEN_HEIGHT)) {
    bufferA[x][y] = bufferA[x][y] | value;
  }
  */
  // LANDSCAPE REVERSE
  /*if ((x>=0) && (x<(SCREEN_WIDTH/8)) && (y>=0) && (y<SCREEN_HEIGHT)) {
    bufferA[(SCREEN_WIDTH/8)-x][SCREEN_HEIGHT-y] |= value;
  }*/
  // PORTRAIT
  /*
   if ((x>=0) && (x<screenWidth) && (y>=0) && (y<uint8_tHeight)) {
    for (short n=0; n<8; n++) {
      if ((value & (1>>n)) != 0)
        bufferA[y/8][(x*8) + n] |= 1<<y;
    }
  }
   */
}

void LM211::setPixel(int x, int y) {
  int offsetX = x / 8; // Count uint8_t offset (buffer stores X as uint8_ts Array)
  int deltaX = x % 8; // count blank bits inside buffer X uint8_t
  // computed values for rotation
  int xp=SCREEN_WIDTH - 1 - y;
  int yp=x;
  
  switch (screenDirection) {
    case top:
      if ((x>=0) && (x<SCREEN_WIDTH) && (y>=0) && (y<SCREEN_HEIGHT)) {
        bufferA[offsetX][y] |= (128 >> deltaX);
      }
      break;
    case bottom:
      // LANDSCAPE REVERSE
      if ((x>=0) && (x<SCREEN_WIDTH) && (y>=0) && (y<SCREEN_HEIGHT)) {
        bufferA[(SCREEN_WIDTH/8)-offsetX-1][SCREEN_HEIGHT-y-1] |= (1<< deltaX);
      }
      break;
    case right:
      // PORTRAIT
      offsetX = xp / 8;
      deltaX = xp % 8;
      if ((offsetX>=0) && (offsetX<(SCREEN_WIDTH/8)) && (yp>=0) && (yp<SCREEN_HEIGHT)) {
        bufferA[offsetX][yp] |= (128 >> deltaX);
      }
      break;
    case left:
      // PORTRAIT REVERSE
      offsetX = xp / 8;
      deltaX = xp % 8;
      if ((offsetX>=0) && (offsetX<(SCREEN_WIDTH/8)) && (yp>=0) && (yp<SCREEN_HEIGHT)) {
        bufferA[(SCREEN_WIDTH/8)-offsetX-1][SCREEN_HEIGHT-yp-1] |= (1 << deltaX);
      }
      break;
  }
  /*
  if ((x>=0) && (x<SCREEN_WIDTH) && (y>=0) && (y<SCREEN_HEIGHT)) {
    bufferA[offsetX][y] |= (128 >> deltaX);
  }
  */
  // LANDSCAPE REVERSE
  /*  
  if ((x>=0) && (x<(SCREEN_WIDTH/8)) && (y>=0) && (y<SCREEN_HEIGHT)) {
    bufferA[(SCREEN_WIDTH/8)-offsetX][SCREEN_HEIGHT-y] |= (1 << deltaX);
  }
  */
  // PORTRAIT
  /*
  int xp=SCREEN_WIDTH - 1 - y;
  int yp=x;
  offsetX = xp / 8;
  deltaX = xp % 8;
  if ((x>=0) && (x<(SCREEN_WIDTH/8)) && (y>=0) && (y<SCREEN_HEIGHT)) {
    bufferA[offsetX][yp] |= (128 >> deltaX);
  }
  */
  // PORTRAIT REVERSE
  /*
  int xp=SCREEN_WIDTH - 1 - y;
  int yp=x;
  offsetX = xp / 8;
  deltaX = xp % 8;
  if ((x>=0) && (x<(SCREEN_WIDTH/8)) && (y>=0) && (y<SCREEN_HEIGHT)) {
    bufferA[(SCREEN_WIDTH/8)-offsetX][SCREEN_HEIGHT-yp] |= (1 << deltaX);
  }
  */
}
void LM211::clearPixel(int x, int y) {
  int offsetX = x / 8; // Count uint8_t offset (buffer stores X as uint8_ts Array)
  int deltaX = x % 8; // count blank bits inside buffer X uint8_t
  // computed values for rotation
  int xp=SCREEN_WIDTH - 1 - y;
  int yp=x;
  
  switch (screenDirection) {
    case top:
      if ((x>=0) && (x<SCREEN_WIDTH) && (y>=0) && (y<SCREEN_HEIGHT)) {
        bufferA[offsetX][y] &= ~(128 >> deltaX);
      }
      break;
    case bottom:
      // LANDSCAPE REVERSE
      if ((x>=0) && (x<SCREEN_WIDTH) && (y>=0) && (y<SCREEN_HEIGHT)) {
        bufferA[(SCREEN_WIDTH/8)-offsetX-1][SCREEN_HEIGHT-y-1] &= ~(1 << deltaX);
      }
      break;
    case right:
      // PORTRAIT
      offsetX = xp / 8;
      deltaX = xp % 8;
      if ((offsetX>=0) && (offsetX<(SCREEN_WIDTH/8)) && (yp>=0) && (yp<SCREEN_HEIGHT)) {
        bufferA[offsetX][yp] &= ~(128 >> deltaX);
      }
      break;
    case left:
      // PORTRAIT REVERSE
      offsetX = xp / 8;
      deltaX = xp % 8;
      if ((offsetX>=0) && (offsetX<(SCREEN_WIDTH/8)) && (yp>=0) && (yp<SCREEN_HEIGHT)) {
        bufferA[(SCREEN_WIDTH/8)-offsetX-1][SCREEN_HEIGHT-yp-1] &= ~(1 << deltaX);
      }
      break;
  }
  /*
  if ((x>=0) && (x<SCREEN_WIDTH) && (y>=0) && (y<SCREEN_HEIGHT)) {
    bufferA[offsetX][y] &= ~(128 >> deltaX);
  }
  */
  // LANDSCAPE REVERSE
  /*
  if ((x>=0) && (x<(SCREEN_WIDTH/8)) && (y>=0) && (y<SCREEN_HEIGHT)) {
    bufferA[(SCREEN_WIDTH/8)-offsetX][SCREEN_HEIGHT-y] &= ~(1 << deltaX);
  }
  */
  // PORTRAIT
  /*
  int xp=SCREEN_WIDTH - 1 - y;
  int yp=x;
  offsetX = xp / 8;
  deltaX = xp % 8;
  if ((x>=0) && (x<(SCREEN_WIDTH/8)) && (y>=0) && (y<SCREEN_HEIGHT)) {
    bufferA[offsetX][yp] &= ~(128 >> deltaX);
  }
  */
  // PORTRAIT REVERSE
  /*
  int xp=SCREEN_WIDTH - 1 - y;
  int yp=x;
  offsetX = xp / 8;
  deltaX = xp % 8;
  if ((x>=0) && (x<(SCREEN_WIDTH/8)) && (y>=0) && (y<SCREEN_HEIGHT)) {
    bufferA[(SCREEN_WIDTH/8)-offsetX][SCREEN_HEIGHT-yp] &= ~(1 << deltaX);
  }
  */
}

/*
 * TODO: ADD ROTATION SUPPORT
 */
void LM211::writePixel(int x, int y, uint8_t color) {
  if (color == 0) {
    clearPixel(x, y);
  } else {
    setPixel(x, y);
  }
}

//
// Breseham algorithms
//
void LM211::plotLine(int x0, int y0, int x1, int y1)
{
   int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
   int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
   int err = dx+dy, e2; /* error value e_xy */
 
   for(;;){  /* loop */
      //setPixel(x0,y0);
      writePixel(x0,y0,1);
      if (x0==x1 && y0==y1) break;
      e2 = 2*err;
      if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
      if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
   }
}
void LM211::plotCircle(int xm, int ym, int r)
{
   int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
   do {
      //setPixel(xm-x, ym+y); /*   I. Quadrant */
      //setPixel(xm-y, ym-x); /*  II. Quadrant */
      //setPixel(xm+x, ym-y); /* III. Quadrant */
      //setPixel(xm+y, ym+x); /*  IV. Quadrant */
      
      writePixel(xm-x, ym+y, 1); /*   I. Quadrant */
      writePixel(xm-y, ym-x, 1); /*  II. Quadrant */
      writePixel(xm+x, ym-y, 1); /* III. Quadrant */
      writePixel(xm+y, ym+x, 1); /*  IV. Quadrant */
      r = err;
      if (r <= y) err += ++y*2+1;           /* e_xy+e_y < 0 */
      if (r > x || err > y) err += ++x*2+1; /* e_xy+e_x > 0 or no 2nd y-step */
   } while (x < 0);
}
void LM211::plotEllipseRect(int x0, int y0, int x1, int y1)
{
   int a = abs(x1-x0), b = abs(y1-y0), b1 = b&1; /* values of diameter */
   long dx = 4*(1-a)*b*b, dy = 4*(b1+1)*a*a; /* error increment */
   long err = dx+dy+b1*a*a, e2; /* error of 1.step */

   if (x0 > x1) { x0 = x1; x1 += a; } /* if called with swapped points */
   if (y0 > y1) y0 = y1; /* .. exchange them */
   y0 += (b+1)/2; y1 = y0-b1;   /* starting pixel */
   a *= 8*a; b1 = 8*b*b;

   do {
       //setPixel(x1, y0); /*   I. Quadrant */
       //setPixel(x0, y0); /*  II. Quadrant */
       //setPixel(x0, y1); /* III. Quadrant */
       //setPixel(x1, y1); /*  IV. Quadrant */
       writePixel(x1, y0, 1); /*   I. Quadrant */
       writePixel(x0, y0, 1); /*  II. Quadrant */
       writePixel(x0, y1, 1); /* III. Quadrant */
       writePixel(x1, y1, 1); /*  IV. Quadrant */
       
       e2 = 2*err;
       if (e2 <= dy) { y0++; y1--; err += dy += a; }  /* y step */ 
       if (e2 >= dx || 2*err > dy) { x0++; x1--; err += dx += b1; } /* x step */
   } while (x0 <= x1);
   
   while (y0-y1 < b) {  /* too early stop of flat ellipses a=1 */
       //setPixel(x0-1, y0); /* -> finish tip of ellipse */
       //setPixel(x1+1, y0++); 
       //setPixel(x0-1, y1);
       //setPixel(x1+1, y1--); 
       writePixel(x0-1, y0, 1); /* -> finish tip of ellipse */
       writePixel(x1+1, y0++, 1); 
       writePixel(x0-1, y1, 1);
       writePixel(x1+1, y1--, 1); 
   }
}
void LM211::plotQuadBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2)
{                            
  int sx = x2-x1, sy = y2-y1;
  long xx = x0-x1, yy = y0-y1, xy;         /* relative values for checks */
  double dx, dy, err, cur = xx*sy-yy*sx;                    /* curvature */

  assert(xx*sx <= 0 && yy*sy <= 0);  /* sign of gradient must not change */

  if (sx*(long)sx+sy*(long)sy > xx*xx+yy*yy) { /* begin with longer part */ 
    x2 = x0; x0 = sx+x1; y2 = y0; y0 = sy+y1; cur = -cur;  /* swap P0 P2 */
  }  
  if (cur != 0) {                                    /* no straight line */
    xx += sx; xx *= sx = x0 < x2 ? 1 : -1;           /* x step direction */
    yy += sy; yy *= sy = y0 < y2 ? 1 : -1;           /* y step direction */
    xy = 2*xx*yy; xx *= xx; yy *= yy;          /* differences 2nd degree */
    if (cur*sx*sy < 0) {                           /* negated curvature? */
      xx = -xx; yy = -yy; xy = -xy; cur = -cur;
    }
    dx = 4.0*sy*cur*(x1-x0)+xx-xy;             /* differences 1st degree */
    dy = 4.0*sx*cur*(y0-y1)+yy-xy;
    xx += xx; yy += yy; err = dx+dy+xy;                /* error 1st step */    
    do {                              
      //setPixel(x0,y0);                                     /* plot curve */
      writePixel(x0,y0,1); 
      if (x0 == x2 && y0 == y2) return;  /* last pixel -> curve finished */
      y1 = 2*err < dx;                  /* save value for test of y step */
      if (2*err > dy) { x0 += sx; dx -= xy; err += dy += yy; } /* x step */
      if (    y1    ) { y0 += sy; dy -= xy; err += dx += xx; } /* y step */
    } while (dy < dx );           /* gradient negates -> algorithm fails */
  }
  plotLine(x0,y0, x2,y2);                  /* plot remaining part to end */
}
void LM211::plotLineWidth(int x0, int y0, int x1, int y1, float wd, uint8_t color)
{ 
   int dx = abs(x1-x0), sx = x0 < x1 ? 1 : -1; 
   int dy = abs(y1-y0), sy = y0 < y1 ? 1 : -1; 
   int err = dx-dy, e2, x2, y2;                          /* error value e_xy */
   float ed = dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);
   
   for (wd = (wd+1)/2; ; ) {                                   /* pixel loop */
      writePixel(x0,y0, color);
      e2 = err; x2 = x0;
      if (2*e2 >= -dx) {                                           /* x step */
         for (e2 += dy, y2 = y0; e2 < ed*wd && (y1 != y2 || dx > dy); e2 += dx)
            writePixel(x0, y2 += sy, color);
         if (x0 == x1) break;
         e2 = err; err -= dy; x0 += sx; 
      } 
      if (2*e2 <= dy) {                                            /* y step */
         for (e2 = dx-e2; e2 < ed*wd && (x1 != x2 || dx < dy); e2 += dy)
           writePixel(x2 += sx, y0, color);
         if (y0 == y1) break;
         err += dx; y0 += sy; 
      }
   }
    
}

void LM211::createFilledCircle(int cx, int cy, int radius, uint8_t color, int percWidth, int beginAngle, int endAngle, bool dir)
{
  
  int degreeOffset=MIN_ANGLE;
  double degree;
  double radiusMax = radius;
  double radiusMin = radius - (radiusMax * percWidth /100);
  if (dir) {
    int degreeMin=beginAngle+degreeOffset;
    if (degreeMin >= 360) { degreeMin -= 360; }
    int degreeMax=endAngle+degreeOffset;
    if (degreeMax >= 360) { degreeMax -= 360; }
    
    if (degreeMax > degreeMin) {
      for (degree=degreeMin; degree<=degreeMax; degree+=1)
      {
        drawArcBand(cx, cy, degree, radiusMax, radiusMin, color);
      }
    } else {
      for (degree=degreeMin; degree<360; degree+=1)
      {
        drawArcBand(cx, cy, degree, radiusMax, radiusMin, color);
      }
      for (degree=0; degree<=degreeMax; degree+=1)
      {
        drawArcBand(cx, cy, degree, radiusMax, radiusMin, color);
      }
    }
  }else {
    int degreeMin=endAngle+degreeOffset;
    if (degreeMin >= 360) { degreeMin -= 360; }
    int degreeMax=beginAngle+degreeOffset;
    if (degreeMax >= 360) { degreeMax -= 360; }
    
    if (degreeMax > degreeMin) {
      for (degree=degreeMax; degree>=degreeMin; degree--)
      {
        drawArcBand(cx, cy, degree, radiusMax, radiusMin, !color);
      }
    } else {
      for (degree=degreeMax; degree>0; degree--)
      {
        drawArcBand(cx, cy, degree, radiusMax, radiusMin, !color);
      }
      for (degree=360; degree>=degreeMin; degree--)
      {
        drawArcBand(cx, cy, degree, radiusMax, radiusMin, !color);
      }
    }
  }
}

void LM211::drawArcBand (int x, int y, double degree, double radiusMax, double radiusMin, uint8_t color) {
  int opposeMax = radiusMax * sin (degree*PI/180);
  int adjacentMax = radiusMax * cos (degree*PI/180);
  int opposeMin = radiusMin * sin (degree*PI/180);
  int adjacentMin = radiusMin * cos (degree*PI/180);
  plotLineWidth(x+adjacentMax, y+opposeMax, x+adjacentMin, y+opposeMin, 1, color);
  //refresh();
  // Borders
  //lcd->writePixel(cx+adjacentMax, cy+opposeMax,1);
  //lcd->writePixel(cx+adjacentMin, cy+opposeMin,1);
}

void LM211::drawRectangle (int x, int y, int width, int height) {
  short offsetX = x / 8; // Count uint8_t offset (buffer stores X as uint8_ts Array)
  short deltaX = x % 8; // count blank bits inside buffer X uint8_t
  short offsetW = (x+width) / 8; // Count uint8_t offset (buffer stores X as uint8_ts Array)
  short deltaW = (x+width) % 8; // count black bits inside buffer X uint8_t
  
  short a;

  // First Line
  writeBuffer(offsetX,y, 0xff >> deltaX);
  writeBuffer(offsetW,y, 0xFF80 >> deltaW);
  for (a = (offsetX +1); a < offsetW; a++) {
    writeBuffer(a,y,0xFF);
  }
  // Other lines
  for (a = (y +1); a < (y + height); a ++) {
    writeBuffer(offsetX,a, 128 >> deltaX);
    writeBuffer(offsetW,a, 128 >> deltaW);
  }
  
  // Last line
  writeBuffer(offsetX,y+height, 0xff >> deltaX);
  writeBuffer(offsetW,y+height, 0xFF80 >> deltaW);  
  for (a = (offsetX +1); a < offsetW; a++) {
    writeBuffer(a,y+height, 0xFF);
  }
}

/*
 * TODO: ADD ROTATION SUPPORT OR USE SUPPORTED FONCTIONS (DO NOT USE SCREEN BUFFER)
 */
void LM211::drawFilledRectangle (int x, int y, int width, int height, bool black) {
  int offsetX = x / 8; // Count uint8_t offset (buffer stores X as uint8_ts Array)
  int deltaX = x % 8; // count blank bits inside buffer X uint8_t
  int offsetW = (x+width) / 8; // Count uint8_t offset (buffer stores X as uint8_ts Array)
  int deltaW = (x+width) % 8; // count black bits inside buffer X uint8_t

  short color1 = 0xff;
  int color2 = 0xff80;
  /*if (!black) {
    color1 = 0xff00;
    color2 = 0x00ff;
  }*/
  
  int a;
  int b;
  // Other lines
  /*
  for (a = y; a <= (y + height); a ++) {
    if (!black) {
      bufferA[offsetX][a] &=  ~(0xff >> deltaX);
    } else {
      bufferA[offsetX][a] |=  (0xff >> deltaX);
    }
    for (b = (offsetX +1); b < offsetW; b++) {
      if (!black) {
        bufferA[b][a] =  0;
      } else {
        bufferA[b][a] =  0xff;
      }
    }
    if (!black) {
      bufferA[offsetW][a] &= ~(color2 >> deltaW);
    } else {
      bufferA[offsetW][a] |= (color2 >> deltaW);
    }
  }
  */
  for (a = y; a <= (y + height); a ++) {
    if (!black) {
      writeBuffer(offsetX,a,(0xff << deltaX));
    } else {
      writeBuffer(offsetX,a,(0xff >> deltaX));
    }
    for (b = (offsetX +1); b < offsetW; b++) {
      if (!black) {
        writeBuffer(b,a,0);
      } else {
        writeBuffer(b,a,0xff);
      }
    }
    if (!black) {
      writeBuffer(offsetW,a,(color2 << deltaW));
    } else {
      writeBuffer(offsetW,a,(color2 >> deltaW));
    }
  }
}


void LM211::myn_delay()
{
  volatile uint8_t i;
  for(i = 0; i < (F_CPU/10000000L); i++)
  {
    asm("nop");
  }
}
