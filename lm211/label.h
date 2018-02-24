#ifndef label_h
#define label_h


#include "Times_New_Roman__14.h"
#include "Times_New_Roman__6.h"
#include "LM211.h"

#define ALIGN_LEFT    0
#define ALIGN_CENTER  1
#define ALIGN_RIGHT   2


class label{
  public:
    label();

    void initialize(LM211 *lcd);
    void setPosition(int x, int y);
    
    void setAlignLeft();
    void setAlignRight();
    void setAlignCenter();

    void setText(char *strText);
    void redraw();
    
    void setFontLarge();
    void setFontSmall();
    
  private:
    // pointer to LCD screen
    LM211 *screen;
    char *labelText;
    // label alignment
    short align;
    // Label position reference
    int refX;
    int refY;
    // Font size
    const struct FONT_DEF *currentFont;
    // Store the size of the label (for refresh purposes)
    int lastWidth = 0;
    int lastHeight = 0;
    int width = 0;
    int height = 0;

    /*
     * Store old value for intelligent refresh
     * 
     */
     int oldRefX;
     int oldRefY;
     String oldLabelText = "";
     const struct FONT_DEF *oldFont;

    void printString(int x,int y, const char *in, const struct FONT_DEF *strcut1, boolean color);
    void printStringRight(int x,int y, const char *in, const struct FONT_DEF *strcut1, boolean color);
    void printStringCenter(int x,int y, const char *in, const struct FONT_DEF *strcut1, boolean color);
};


#endif

