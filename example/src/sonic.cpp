#include "sonic.h"
#include "config.h"
#include "led_ctrl.h"

extern float distance;

UltrasonicSensor::UltrasonicSensor(uint8_t trigPin, uint8_t echoPin) 
    : trigPin(trigPin), echoPin(echoPin), duration_us(0), distance_cm(0) {}

void UltrasonicSensor::begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

float UltrasonicSensor::getDistance() {
    // generate 10-microsecond pulse to TRIG pin
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // measure duration of pulse from ECHO pin
    duration_us = pulseIn(echoPin, HIGH);

    // calculate the distance
    distance_cm = 0.017 * duration_us;

    delay(50);

    return distance_cm;
}

void UltrasonicSensor::blink(float dis) {

    if (dis < 35) {
            unsigned long blinkInterval = 990 * dis * dis / (35 * 35) + 10;
            static unsigned long lastBlinkTime = 0;
            if (millis() - lastBlinkTime >= blinkInterval) {
                setRed();
                lastBlinkTime = millis();
            }
        } else {
       
        }
}

// 修改任务实现
void UltrasonicSensor::getDistanceTask(void *pvParameters) {
    UltrasonicSensor* sensor = (UltrasonicSensor*)pvParameters;
    
    // 添加任务初始化延时，等待其他系统初始化完成
    vTaskDelay(pdMS_TO_TICKS(100));
    
    while(1) {
        try {
            float current_distance = sensor->getDistance();
            
            if (current_distance >= 0 && current_distance < 400) {
                distance = current_distance;  // 更新全局变量
                sensor->blink(distance);
                Serial.printf("Distance: %.2f cm\n", distance);
            }
            
        } catch (...) {
            Serial.println("Error in sonic task");
        }
        
        // 增加延时时间
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}