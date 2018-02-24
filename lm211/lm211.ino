#include "LM211.h"
#include "widget.h"

#define MY_BAUD_RATE 9600

LM211 lcd;

widget g6;
int toggle = 0;


void setup() {
	Serial.begin(MY_BAUD_RATE);

	lcd.printString(0, 16, "Setup ...", &Times_New_Roman__6, 1);
	lcd.refresh();

	// Clear Screen

	g6.initialize(&lcd, 64, 0, 0, 2000, "Screen", "fps", "Refresh");
	g6.setHidden(false);
	g6.redraw();

	g6.setValue(0);

}


unsigned long timeRef = 0;
unsigned int frameRate = 0;
void loop() {
	if (millis() - timeRef > 1000) {
		g6.setValue(frameRate);
		Serial.println(frameRate);
		frameRate = 0;
		timeRef = millis();
	}

	lcd.refresh();
	frameRate++;
}