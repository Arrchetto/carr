#pragma once
#include <Arduino.h>
#include "config.h"

enum LEDMode {
    LED_OFF,
    LED_RED_SOLID,
    LED_GREEN_SOLID,
    LED_BLUE_SOLID,
    LED_WHITE_SOLID,
    LED_RED_BLINK,
    LED_GREEN_BLINK,
    LED_BLUE_BLINK
};

class LEDController {
public:
    static void begin();
    static void setMode(LEDMode mode);
    static void LEDControlTask(void* pvParameters);
    
private:
    static void setRed();
    static void setGreen();
    static void setBlue();
    static void setWhite();
    static void turnOffLights();
    
    static volatile LEDMode currentMode;
    static const uint32_t BLINK_INTERVAL = 500; // 闪烁间隔(ms)
};