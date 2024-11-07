#pragma once

#include "config.h"

extern bool ledState;
extern bool alarmActive;
extern unsigned long lastBlinkTime;

void setupLEDs();
void setRed();
void setGreen();
void setBlue();
void setWhite();
void turnOffLights();
void updateLEDState();

