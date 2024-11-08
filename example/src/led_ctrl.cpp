#include "led_ctrl.h"
#include "config.h"

volatile LEDMode LEDController::currentMode = LED_OFF;

void LEDController::begin() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    turnOffLights();
    
    // 创建LED控制任务
    xTaskCreate(
        LEDControlTask,
        "LED_Task",
        2048,
        NULL,
        1,
        NULL
    );

    LEDController::setMode(LED_GREEN_BLINK);
}

void LEDController::setMode(LEDMode mode) {
    currentMode = mode;
}

void LEDController::LEDControlTask(void* pvParameters) {
    bool ledState = false;
    TickType_t lastWakeTime = xTaskGetTickCount();
    
    while(1) {
        switch(currentMode) {
            case LED_OFF:
                turnOffLights();
                break;
                
            case LED_RED_SOLID:
                setRed();
                break;
                
            case LED_GREEN_SOLID:
                setGreen();
                break;
                
            case LED_BLUE_SOLID:
                setBlue();
                break;
                
            case LED_WHITE_SOLID:
                setWhite();
                break;
                
            case LED_RED_BLINK:
                ledState = !ledState;
                if(ledState) setRed();
                else turnOffLights();
                break;
                
            case LED_GREEN_BLINK:
                ledState = !ledState;
                if(ledState) setGreen();
                else turnOffLights();
                break;
                
            case LED_BLUE_BLINK:
                ledState = !ledState;
                if(ledState) setBlue();
                else turnOffLights();
                break;
            case LED_WHITE_BLINK:
                ledState = !ledState;
                if(ledState) setWhite();
                else turnOffLights();
                break;
        }
        
        // 使用vTaskDelayUntil确保精确的时间间隔
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(BLINK_INTERVAL));
    }
}

void LEDController::setRed() {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
}

void LEDController::setGreen() {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
}

void LEDController::setBlue() {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
}

void LEDController::setWhite() {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);
}

void LEDController::turnOffLights() {

    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
}

