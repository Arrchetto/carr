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

