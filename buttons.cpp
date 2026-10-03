#include <Arduino.h>
#include "buttons.h"
#include "config.h"
#include "state.h"
#include "messages.h"
#include "display.h"
#include "password.h"

// ---------- Button 4 (short / long press) ----------
static unsigned long pressStart = 0;
static bool buttonPressed = false;
static bool longPress = false;

// ---------- Button 5 (debounced) ----------
static bool lastBtn2State = HIGH;
static unsigned long lastBtn2Time = 0;

void initButtons() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
}

void handleButton5() {
  bool btn2State = digitalRead(BUTTON2_PIN);

  if (btn2State != lastBtn2State && millis() - lastBtn2Time > 50) {
    lastBtn2Time = millis();
    lastBtn2State = btn2State;

    if (btn2State == LOW) {
      if (unlocked) {
        lockDevice();
      } else {
        handlePasswordInput(5);
      }
    }
  }
}

void handleButton4() {
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && !buttonPressed) {
    buttonPressed = true;
    longPress = false;
    pressStart = millis();
  }

  if (buttonPressed && buttonState == LOW) {
    if (unlocked && millis() - pressStart >= 2000 && !longPress) {
      currentMessage = -1;
      showLoveMessage();
      longPress = true;
    }
  }

  if (buttonPressed && buttonState == HIGH) {
    unsigned long pressTime = millis() - pressStart;

    if (pressTime > 30 && !longPress) {
      if (!unlocked) {
        handlePasswordInput(4);
      } else {
        // pick a random message (never the same one twice in a row)
        int newIndex;
        do {
          newIndex = random(totalMessages);   // 0 to 499
        } while (newIndex == currentMessage);

        currentMessage = newIndex;
        showMessage(messages[currentMessage]);
      }
    }

    buttonPressed = false;
  }
}
