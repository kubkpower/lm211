#include "widget.h"

widget::widget() {
  
}

void widget::initialize(LM211 *lcd, int x, int y, int minValue, int maxValue, char *title, char *unit, char *lbl) {
  inScreen = lcd;
  inX = x;
  inY = y;
  inMinValue = minValue;
  inMaxValue = maxValue;
  inHidden = false;
  
  inUnit = unit;
  inValue = minValue;
  
  setLabel(lbl);
  setTitle(title);
  
  myValue.initialize(inScreen);
  myValue.setPosition(inX+(WIDGET_WIDTH/2), inY + (WIDGET_WIDTH/2) - 6);
  myValue.setAlignCenter();
  if (!lbl) {
    myValue.setFontLarge();
    setLabel(inUnit);
    myLabel.setFontLarge();
    myLabel.setPosition(inX+(WIDGET_WIDTH/2), inY + (WIDGET_WIDTH/2) + 11);
    myLabel.setText(inLabel);
  }
  
  
  // Gauge Border
  //Serial.println("Generate Gauge border");
  inScreen->createFilledCircle(inX+(WIDGET_WIDTH/2), inY + (WIDGET_HEIGHT/2), GAUGE_RADIUS+1, 1, 1, 0, MAX_ANGLE-MIN_ANGLE, true);
  // Compute angleMargin
  //Serial.println("Compute angleMargin");
  
  if (MIN_ANGLE > MAX_ANGLE) {
    angleMargin = (360 - MIN_ANGLE) + (MAX_ANGLE);
  } else {
    angleMargin = MAX_ANGLE - MIN_ANGLE;
  }

  
  redraw();
}

void widget::setPosition(int x, int y) {
  // Clears old screen area
  if (!inHidden)
    inScreen->drawFilledRectangle(inX, inY, WIDGET_WIDTH, WIDGET_WIDTH, 0);
  // Load new position
  inX = x;
  inY = y;
  // Draw widget
  redraw();
}

void widget::setHidden(bool state) {
  inHidden = state;
  if (inHidden)
    inScreen->drawFilledRectangle(inX, inY, WIDGET_WIDTH, WIDGET_WIDTH, 0);
}
void widget::setValue(int value) {
  inValue = value;
  redraw();
}
void widget::setTitle(char *title) {
  if (title) {
    inTitle = title;
    myTitle.initialize(inScreen);
    myTitle.setPosition(inX+(WIDGET_WIDTH/2), inY + WIDGET_WIDTH - 4);
    myTitle.setAlignCenter();
    myTitle.setText(inTitle);
  }
}
void widget::setLabel(char *label) {
  if (label) {
    inLabel = label;
    myLabel.initialize(inScreen);
    myLabel.setPosition(inX+(WIDGET_WIDTH/2), inY + (WIDGET_WIDTH/2) + 5);
    myLabel.setAlignCenter();
    myLabel.setText(inLabel);
  }
}
void widget::setUnit(char *unit) {
  inUnit = unit;
}

void widget::redraw() {
  
  // Nothing to draw if hidden
  if (inHidden) {
    return;
  }
  // Draw gauge level
  int useValue = inValue;
  if (useValue > inMaxValue) { useValue = inMaxValue; }
  if (useValue < inMinValue) { useValue = inMinValue; }
  // Proportion d'angle à couvrir
  double moveAngle = angleMargin*(useValue-inMinValue)/(inMaxValue-inMinValue);
  bool moveForward = true;
  if (moveAngle == lastMoveAngle) { return;}
  if (moveAngle < lastMoveAngle) { moveForward = false;}
  

  // Display value
  inScreen->createFilledCircle(inX+(WIDGET_WIDTH/2), inY + (WIDGET_WIDTH/2), GAUGE_RADIUS, 1, CIRCLE_WIDTH_PERC, lastMoveAngle, moveAngle, moveForward);
  lastMoveAngle = moveAngle;
  char strVal[5];
  //inScreen->drawFilledRectangle(inX-10, inY-10,18, 12, 0);
  if (inLabel == inUnit) {
    sprintf(strVal, "%d", useValue);
  } else {
    sprintf(strVal, "%d%s", useValue, inUnit);
  }
  myValue.setText(strVal);

    
  
}

