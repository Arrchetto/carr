#pragma once
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

extern Adafruit_MPU6050 mpu;

extern float angleX, angleY, angleZ;
extern float gyroX, gyroY, gyroZ;
extern float accX, accY, accZ;
extern float lastAngleX, lastAngleY, lastAngleZ;
extern float gyroErrorX, gyroErrorY, gyroErrorZ;


void setupMPU6050();
void calibrateGyro();
void calculateAngle();

// 获取角度数据
float getMPUAngleX();
float getMPUAngleY();
float getMPUAngleZ();

// 获取角速度数据
float getMPUGyroX();
float getMPUGyroY();
float getMPUGyroZ();

// 获取加速度数据
float getMPUAccelX();
float getMPUAccelY();
float getMPUAccelZ();

void updateMPUData(void *pvParameters);

