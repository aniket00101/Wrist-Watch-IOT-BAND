#include <Arduino.h>
#include "password.h"
#include "state.h"
#include "display.h"
#include "eyes.h"

static const int password[] = {4, 5, 5, 4, 5, 4, 4};
static const int passwordLength = 7;
static int passwordIndex = 0;

void handlePasswordInput(int button) {
  if (button == password[passwordIndex]) {
    passwordIndex++;
  } else {
    passwordIndex = (button == password[0]) ? 1 : 0;
  }

  if (passwordIndex == passwordLength) {
    passwordIndex = 0;
    unlocked = true;
    currentMessage = -1;
    showLoveMessage();
  }
}

void lockDevice() {
  unlocked = false;
  passwordIndex = 0;
  currentMessage = -1;
  resetEyes();
}
