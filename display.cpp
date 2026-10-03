#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display.h"
#include "config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void showLoveMessage() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);

  display.setCursor(5, 10);
  display.println("I LOVE YOU");

  display.setCursor(21, 35);
  display.println("DISHARI");

  display.display();
}

void showMessage(const char* message) {
  const int charW = 6;
  const int charH = 8;
  const int maxChars = SCREEN_WIDTH / charW;  // 21 characters per line
  const int maxLines = 6;

  // ---- Word wrap into lines ----
  String lines[maxLines];
  int lineCount = 0;
  String text(message);
  String current = "";
  int pos = 0;

  while (pos < (int)text.length() && lineCount < maxLines) {
    int space = text.indexOf(' ', pos);
    if (space == -1) space = text.length();
    String word = text.substring(pos, space);
    pos = space + 1;

    if (current.length() == 0) {
      current = word;
    } else if ((int)(current.length() + 1 + word.length()) <= maxChars) {
      current += " " + word;
    } else {
      lines[lineCount++] = current;
      current = word;
    }
  }
  if (current.length() > 0 && lineCount < maxLines) {
    lines[lineCount++] = current;
  }

  // ---- Work out the gap ----
  int gap = LINE_GAP;
  if (SPREAD_TO_BOTTOM && lineCount > 1) {
    gap = (SCREEN_HEIGHT - lineCount * charH) / (lineCount - 1);
    if (gap > MAX_GAP) gap = MAX_GAP;
    if (gap < 1) gap = 1;
  }

  // ---- Draw from the top ----
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);

  int y = 0;   // start at the top
  for (int i = 0; i < lineCount; i++) {
    display.setCursor(0, y);
    display.print(lines[i]);
    y += charH + gap;
  }

  display.display();
}
