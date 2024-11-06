#include "mpu6050.h"
#include "config.h"

float gyroX, gyroY, gyroZ;
float accX, accY, accZ;
float angleX, angleY, angleZ;
float lastAngleX = 0, lastAngleY = 0, lastAngleZ = 0;
float gyroErrorX = 0, gyroErrorY = 0, gyroErrorZ = 0;

Adafruit_MPU6050 mpu;
TwoWire I2C_MPU(1);

void setupMPU6050() {
    I2C_MPU.begin(I2C_SDA, I2C_SCL);

    if (!mpu.begin(0x68, &I2C_MPU)) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
}

void calibrateGyro() {
  float sumX = 0, sumY = 0, sumZ = 0;
  int numSamples = 1000;

  for (int i = 0; i < numSamples; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    sumX += g.gyro.x;
    sumY += g.gyro.y;
    sumZ += g.gyro.z;
    delay(1);
  }

  gyroErrorX = sumX / numSamples;
  gyroErrorY = sumY / numSamples;
  gyroErrorZ = sumZ / numSamples;
}

void calculateAngle() {
  static unsigned long lastTime = 0;
  unsigned long currentTime = micros();
  float dt = (currentTime - lastTime) / 1000000.0;
  lastTime = currentTime;

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // 应用陀螺仪误差校正
  float gyroX = (g.gyro.x - gyroErrorX) * GYRO_SCALE;
  float gyroY = (g.gyro.y - gyroErrorY) * GYRO_SCALE;
  float gyroZ = (g.gyro.z - gyroErrorZ) * GYRO_SCALE;

  // 计算加速度计角度
  float accelAngleX = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
  float accelAngleY = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180 / PI;
  float accelAngleZ = atan2(sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y), a.acceleration.z) * 180 / PI;

  // 应用互补滤波器
  angleX = ALPHA * (lastAngleX + gyroX * dt) + (1 - ALPHA) * accelAngleX;
  angleY = ALPHA * (lastAngleY + gyroY * dt) + (1 - ALPHA) * accelAngleY;
  angleZ = ALPHA * (lastAngleZ + gyroZ * dt) + (1 - ALPHA) * accelAngleZ;

  // 更新上一次的角度
  lastAngleX = angleX;
  lastAngleY = angleY;
  lastAngleZ = angleZ;

  // 限制角度范围在 -180 到 180 度之间
  angleX = fmod(angleX + 180, 360) - 180;
  angleY = fmod(angleY + 180, 360) - 180;
  angleZ = fmod(angleZ + 180, 360) - 180;
}

// ... 实现 setupMPU6050(), calibrateGyro(), calculateAngle() 等函数 ...

