#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <U8g2lib.h>
#include "sonic.h"

#define I2C_SDA 11
#define I2C_SCL 10
#define TRIG_PIN 15 
#define ECHO_PIN 16 
#define RED_PIN 42
#define GREEN_PIN 41
#define BLUE_PIN 40
#define CRUSH_PIN 2
#define BEEP_PIN 1
#define MOTOR_A1 20
#define MOTOR_A2 21
#define MOTOR_B1 13
#define MOTOR_B2 14
#define RADAR_PIN 7

#define ALPHA 0.96
#define GYRO_SCALE 0.0174533 // 度到弧度的转换因子 (PI/180)
//#define BLINK_INTERVAL 500 // 闪烁间隔，单位为毫秒

extern const char* ssid ;
extern const char* password ;

extern TimerHandle_t wifi_timer;

const int pwmPin1 = 13;
const int pwmPin2 = 14;
const int pwmPin3 = 20;
const int pwmPin4 = 21;

const int pwmFreq = 5000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int pwmChannel3 = 2;
const int pwmChannel4 = 3;
const int pwmResolution = 8;

extern bool beep_flag;

class SystemCheck {
public:
    static void begin();
    static void selfCheckTask(void* pvParameters);
    static bool isCrushDetected() { return isCrush; }
    static float getDistance() { return distance; }

private:
    static volatile bool isCrush;
    static volatile float distance;
    static const uint32_t CHECK_INTERVAL = 100; // 检测间隔(ms)
    static UltrasonicSensor* sonicSensor;
};

void initGPIO();

