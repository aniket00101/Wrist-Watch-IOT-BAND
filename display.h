#pragma once
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

void showLoveMessage();
void showMessage(const char* message);
