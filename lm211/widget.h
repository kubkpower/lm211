#ifndef widget_h
#define widget_h


#include "Times_New_Roman__14.h"
#include "Times_New_Roman__6.h"
#include "LM211.h"
#include "label.h"

#define WIDGET_WIDTH 64
#define WIDGET_HEIGHT 64

// GAUGE Parameters
#define MIN_ANGLE 135
#define MAX_ANGLE 45
#define CIRCLE_WIDTH_PERC 15
#define GAUGE_RADIUS 30


class widget{
  public:
    widget();
    
    void initialize(LM211 *lcd, int x, int y, int minValue, int maxValue, char *title, char *unit, char *label);

    void setPosition(int x, int y);
    void setHidden(bool state);

    void setValue(int value);
    void setTitle(char *title);
    void setLabel(char *label);
    void setUnit(char *unit);
    void redraw();
    
  private:
    // pointer to LCD screen
    LM211 *inScreen;
    int inX;
    int inY;
    int inMinValue;
    int inMaxValue;
    char *inLabel;
    char *inTitle;
    char *inUnit;
    int inValue;
    bool inHidden;

    label myTitle;
    label myLabel;
    label myValue;

    // Gauge parameters
    double lastMoveAngle = 0;
    double angleMargin;
};


#endif

