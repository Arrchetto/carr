#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <U8g2lib.h>

#define I2C_SDA 11
#define I2C_SCL 10
#define RED_PIN 42
#define GREEN_PIN 41
#define BLUE_PIN 40
#define ALPHA 0.96
#define GYRO_SCALE 0.0174533 // 度到弧度的转换因子 (PI/180)
#define BLINK_INTERVAL 500 // 闪烁间隔，单位为毫秒

extern const char* ssid ;
extern const char* password ;

extern TimerHandle_t wifi_timer;

const int pwmPin1 = 15;
const int pwmPin2 = 16;
const int pwmFreq = 5000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int pwmResolution = 8;

void initGPIO();

