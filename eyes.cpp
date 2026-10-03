#include <Arduino.h>
#include "eyes.h"
#include "display.h"

// ---------- Eyes animation ----------
static const int EYE_W = 36;
static const int EYE_H = 40;
static const int EYE_Y = 32;
static const int EYE_LX = 36;
static const int EYE_RX = 92;

static float eyeX = 0, eyeY = 0;            // current look offset
static float targetX = 0, targetY = 0;      // where the eyes want to look
static float hL = EYE_H, hR = EYE_H;        // current eye heights
static float targetHL = EYE_H, targetHR = EYE_H;

static bool happy = false;
static bool blinking = false;
static bool winkOnly = false;

static unsigned long lastFrame = 0;
static unsigned long nextBlink = 0, blinkEnd = 0;
static unsigned long nextLook = 0;
static unsigned long nextHappy = 0, happyEnd = 0;

static void drawEye(int cx, float h, bool isHappy) {
  int x = cx - EYE_W / 2 + (int)eyeX;
  int y = EYE_Y - (int)(h / 2) + (int)eyeY;
  int r = min(10, (int)(h / 2));
  if (r < 1) r = 1;

  display.fillRoundRect(x, y, EYE_W, (int)h, r, SSD1306_WHITE);

  if (isHappy) {
    // cut the bottom part away to make a happy "^" arch
    display.fillCircle(cx + (int)eyeX, EYE_Y + (int)eyeY + 26, 20, SSD1306_BLACK);
  }
}

static void drawEyes() {
  display.clearDisplay();
  drawEye(EYE_LX, hL, happy);
  drawEye(EYE_RX, hR, happy);
  display.display();
}

void resetEyes() {
  unsigned long now = millis();
  eyeX = eyeY = targetX = targetY = 0;
  hL = hR = targetHL = targetHR = EYE_H;
  happy = false;
  blinking = false;
  winkOnly = false;
  nextBlink = now + random(1500, 3500);
  nextLook = now + random(800, 2000);
  nextHappy = now + random(4000, 8000);
  lastFrame = 0;
}

void updateEyes() {
  unsigned long now = millis();
  if (now - lastFrame < 40) return;      // ~25 frames per second
  lastFrame = now;

  // Look around
  if (now >= nextLook) {
    targetX = random(-10, 11);
    targetY = random(-5, 6);
    nextLook = now + random(1000, 3000);
  }

  // Blink or wink
  if (!blinking && now >= nextBlink) {
    blinking = true;
    winkOnly = (random(5) == 0);
    blinkEnd = now + (winkOnly ? 350 : 150);
    nextBlink = now + random(2000, 5000);
  }
  if (blinking && now >= blinkEnd) {
    blinking = false;
  }

  // Happy face
  if (!happy && now >= nextHappy) {
    happy = true;
    happyEnd = now + 1800;
    nextHappy = now + random(5000, 9000);
  }
  if (happy && now >= happyEnd) {
    happy = false;
  }

  // Target heights
  targetHL = EYE_H;
  targetHR = EYE_H;
  if (blinking && !happy) {
    targetHR = 4;                        // right eye always closes
    if (!winkOnly) targetHL = 4;         // left eye only for a full blink
  }

  // Smooth movement
  eyeX += (targetX - eyeX) * 0.35;
  eyeY += (targetY - eyeY) * 0.35;
  hL += (targetHL - hL) * 0.6;
  hR += (targetHR - hR) * 0.6;

  drawEyes();
}
