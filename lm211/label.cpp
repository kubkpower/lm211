#include "label.h"

label::label() {
  setAlignLeft();
  currentFont = &Times_New_Roman__6;
  refX = 0;
  refY = 0;
}

void label::initialize(LM211 *lcd) {
  screen = lcd;
}


//
// Set reference position
//

void label::setPosition(int x, int y) {
  refX = x;
  refY = y;
}

//
// Alignment
//

void label::setAlignLeft() {
  align = ALIGN_LEFT;
}

void label::setAlignCenter() {
  align = ALIGN_CENTER;
}

void label::setAlignRight() {
  align = ALIGN_RIGHT;
}


void label::setFontLarge(){
  currentFont = &Times_New_Roman__14;  
}

void label::setFontSmall(){
  currentFont = &Times_New_Roman__6;
}

void label::setText(char *strText) {
  labelText = strText;
  redraw();
}

void label::redraw() {
  // Label has changed, remove old label
  
  if (oldLabelText != labelText) {
    switch (align) {
      case ALIGN_RIGHT:
        //screen->drawFilledRectangle(refX-width, refY-height, width, height, 0);
        printStringRight(oldRefX,oldRefY, oldLabelText.c_str(), oldFont, 0);
        break;
      case ALIGN_CENTER:
        //screen->drawFilledRectangle(refX-(width/2), refY-(height/2), width, height, 0);
        printStringCenter(oldRefX,oldRefY, oldLabelText.c_str(), oldFont, 0);
        break;
      default:
        // Align left
        //screen->drawFilledRectangle(refX, refY, width, height, 0);
        printString(oldRefX,oldRefY, oldLabelText.c_str(), oldFont, 0);
    }
    
  }
  
  // Update Reference values
  oldRefX = refX;
  oldRefY = refY;
  oldLabelText = labelText;
  oldFont = currentFont;
  
  // Write new string
  switch (align) {
    case ALIGN_RIGHT:
      //screen->drawFilledRectangle(refX-width, refY-height, width, height, 0);
      printStringRight(refX,refY, labelText, currentFont, 1);
      break;
    case ALIGN_CENTER:
      //screen->drawFilledRectangle(refX-(width/2), refY-(height/2), width, height, 0);
      printStringCenter(refX,refY, labelText, currentFont, 1);
      break;
    default:
      // Align left
      //screen->drawFilledRectangle(refX, refY, width, height, 0);
      printString(refX,refY, labelText, currentFont, 1);
  }
  
}

void label::printString(int x,int y, const char *in, const struct FONT_DEF *strcut1, boolean color)
{
  screen->printString( x, y, in, strcut1, color);
  /*
  unsigned int offset;
  unsigned char txtwidth, by=0, mask=0;
  unsigned char NrBytes;
  unsigned char i,j,map,allwidth=0;

  while((map = *in++)) 
  {
    map = pgm_read_byte(&strcut1->mapping_table[map]);

    txtwidth = strcut1->glyph_width;
    if(txtwidth == 0)
      txtwidth = pgm_read_byte(&strcut1->width_table[map]);

    offset = pgm_read_word(&strcut1->offset_table[map]);
    height = strcut1->glyph_height;

        NrBytes = ((txtwidth-1)/8)+1;
  
    for(j=0; j<height * NrBytes; j+=NrBytes    )
    {   // height
      for(i=0 ; i<txtwidth  ; i++)
      {   //  width
          if(i%8 == 0)
          {
           by = pgm_read_byte(&strcut1->glyph_table[ offset+j+(i/8) ]);
           mask = 0x80;
          }

        if( by & mask )
          screen->setPixel(  x+i+allwidth , y+j/ NrBytes);
        else
          screen->clearPixel(  x+i+allwidth , y+j/ NrBytes);

        mask >>= 1;
      }//End i
    }// End j
    allwidth+=txtwidth;
  }// End K

  // Store at object level
  width = allwidth;
  */
} 

void label::printStringRight(int x,int y, const char *in, const struct FONT_DEF *strcut1, boolean color)
{
  screen->printStringRight( x, y, in, strcut1, color);
  /*
  unsigned char txtwidth;
  unsigned char map,allwidth=0;
  const char *ref = in;

  while((map = *in++)) 
  {
    map = pgm_read_byte(&strcut1->mapping_table[map]);

    txtwidth = strcut1->glyph_width;
    if(txtwidth == 0)
      txtwidth = pgm_read_byte(&strcut1->width_table[map]);

    
    allwidth+=txtwidth;
  }// End K

  printString(x-allwidth, y, ref, strcut1, invers);
  */
} 

void label::printStringCenter(int x,int y, const char *in, const struct FONT_DEF *strcut1, boolean color)
{
  unsigned char txtwidth;
  unsigned char map,allwidth=0;
  const char *ref = in;

  while((map = *in++)) 
  {
    map = pgm_read_byte(&strcut1->mapping_table[map]);

    txtwidth = strcut1->glyph_width;
    height = strcut1->glyph_height;
    if(txtwidth == 0)
      txtwidth = pgm_read_byte(&strcut1->width_table[map]);

    
    allwidth+=txtwidth;
  }// End K

  printString(x-(allwidth/2), y-(height/2), ref, strcut1, color);
} 

