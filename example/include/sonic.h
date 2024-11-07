#pragma once

#include <Arduino.h>

class UltrasonicSensor {
public:
    UltrasonicSensor(uint8_t trigPin, uint8_t echoPin);
    void begin();
    float getDistance();
    void blink(float dis);
    static void getDistanceTask(void *pvParameters);  // 修改为静态方法
private:
    uint8_t trigPin;
    uint8_t echoPin;
    long duration_us;
    float distance_cm;
};

extern UltrasonicSensor ultrasonic;  // 声明外部对象

