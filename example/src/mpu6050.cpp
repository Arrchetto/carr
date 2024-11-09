#include "mpu6050.h"
#include "config.h"
#include "BLEHandler.h"
#include "led_ctrl.h"
float gyroX, gyroY, gyroZ;
float accX, accY, accZ;
float angleX, angleY, angleZ;
float lastAngleX = 0, lastAngleY = 0, lastAngleZ = 0;
float gyroErrorX = 0, gyroErrorY = 0, gyroErrorZ = 0;

Adafruit_MPU6050 mpu;
TwoWire I2C_MPU(1);

bool mpuInitialized = false; // 标志位，指示MPU6050是否成功初始化

const float CRUSH_THRESHOLD = 3.0f;    // Z轴与Y轴差值阈值
const float COLLISION_THRESHOLD = 10.0f; // 碰撞加速度阈值
const int COLLISION_WINDOW = 5;         // 碰撞检测窗口(ms)

// 用于存储上一次的加速度值
float lastAccY = 0;
unsigned long lastAccTime = 0;

bool checkCrushCondition(float accY, float accZ) {
    float diff = abs(accZ - accY);
    return diff < CRUSH_THRESHOLD;
}

bool checkCollisionCondition(float currentAccY) {
    unsigned long currentTime = millis();
    
    // 检查时间窗口
    if (currentTime - lastAccTime <= COLLISION_WINDOW) {
        // 计算加速度变化
        float accChange = abs(currentAccY - lastAccY);
        if (accChange > COLLISION_THRESHOLD) {
            lastAccY = currentAccY;
            lastAccTime = currentTime;
            return true;
        }
    }
    
    // 更新历史数据
    lastAccY = currentAccY;
    lastAccTime = currentTime;
    return false;
}

void setupMPU6050() {
    I2C_MPU.begin(I2C_SDA, I2C_SCL);

    if (!mpu.begin(0x68, &I2C_MPU)) {
        Serial.println("Sensor init failed");
        mpuInitialized = false; // 初始化失败
    } else {
        Serial.println("MPU6050 Found!");
        mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
        mpu.setGyroRange(MPU6050_RANGE_250_DEG);
        mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
        mpuInitialized = true; // 初始化成功
    }
}

void calibrateGyro() {
    if (!mpuInitialized) {
        Serial.println("MPU6050 not initialized");
        return;
    }

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
    if (!mpuInitialized) {
        Serial.println("MPU6050 not initialized");
        return;
    }

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

// ... existing code ...

// 获取角度数据
float getMPUAngleX() { return angleX; }
float getMPUAngleY() { return angleY; }
float getMPUAngleZ() { return angleZ; }

// 获取角速度数据（已校正的陀螺仪数据）
float getMPUGyroX() {
    if (!mpuInitialized) return 0;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    return (g.gyro.x - gyroErrorX) * GYRO_SCALE;
}

float getMPUGyroY() {
    if (!mpuInitialized) return 0;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    return (g.gyro.y - gyroErrorY) * GYRO_SCALE;
}

float getMPUGyroZ() {
    if (!mpuInitialized) return 0;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    return (g.gyro.z - gyroErrorZ) * GYRO_SCALE;
}

// 获取加速度数据
float getMPUAccelX() {
    if (!mpuInitialized) return 0;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    return a.acceleration.x;
}

float getMPUAccelY() {
    if (!mpuInitialized) return 0;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    return a.acceleration.y;
}

float getMPUAccelZ() {
    if (!mpuInitialized) return 0;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    return a.acceleration.z;
}

void updateMPUData(void *pvParameters) {
    // 初始化变量
    lastAccTime = millis();
    lastAccY = 0;
    
    while(1) {
        if (mpuInitialized) {
            //calculateAngle();

            // 获取所有数据

            // 打印或发送数据
            Serial.printf("%.2f,%.2f,%.2f\n", accX, accY, accZ);
            //Serial.printf("Angle: P=%.2f R=%.2f Y=%.2f\n", pitch, roll, yaw);
            //Serial.printf("Gyro: X=%.2f Y=%.2f Z=%.2f\n", gyroX, gyroY, gyroZ);

            // 可以在这里添加BLE发送数据的代码
            //BLEHandler::sendData(std::to_string(gyroX) + "," + std::to_string(gyroY) + "," + std::to_string(gyroZ));
        vTaskDelay(pdMS_TO_TICKS(5));  // 5ms采样间隔
    }
}


