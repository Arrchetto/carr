#include "led_ctrl.h"
#include "config.h"

bool ledState = false;
extern bool alarmActive;
unsigned long lastBlinkTime = 0;

// ... 实现 LED 控制相关函数 ...

void updateLEDState() {
  if (alarmActive) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = currentMillis;
      ledState = !ledState;
      if (ledState) {
        setRed();
      } else {
        turnOffLights();
      }
    }
  } else {
    setGreen();
  }
}

void setRed() {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

void setGreen() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);
}

void setBlue() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
}

void setWhite() {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
}

void turnOffLights() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}
