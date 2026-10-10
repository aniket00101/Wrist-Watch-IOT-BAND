#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "config.h"
#include "state.h"
#include "display.h"
#include "eyes.h"
#include "buttons.h"

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);

  initButtons();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  randomSeed(micros());
  resetEyes();
}

void loop() {
  if (!unlocked) {
    updateEyes();
  }

  handleButton5();
  handleButton4();
}
