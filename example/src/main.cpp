#include <Arduino.h>
#include "BLEHandler.h"
#include "WiFi_Server.h"
#include "oled.h"
#include "mpu6050.h"
#include "sonic.h"
#include "led_ctrl.h"

#define wifi_flag 1
#define ble_flag 0
#define mpu_flag 0
#define oled_flag 0
#define Sonic_flag 1
#define Servo_flag 1

float distance;

OLED oled;
UltrasonicSensor ultrasonic(TRIG_PIN, ECHO_PIN);

void setup() {

    LEDController::begin();
    
    #if wifi_flag

    setupWiFi();
    wifi_timer = xTimerCreate("WiFiTimer", pdMS_TO_TICKS(2000), pdTRUE, (void*)0, checkWiFiConnection);
    xTimerStart(wifi_timer, 0);

    #endif

    #if Sonic_flag
        ultrasonic.begin();
    #endif

    oled.setup();

    #if mpu_flag
        setupMPU6050();

        xTaskCreate(
        updateMPUData,       // 任务函数
        "MPU Task",          // 任务名称
        4096,                // 任务堆栈大小
        NULL,                // 任务参数
        1,                   // 任务优先级
        NULL                 // 任务句柄
    );
    #endif

    xTaskCreate(
        BLEHandler::BLETask, // 任务函数
        "BLE Task",          // 任务名称
        4096,                // 任务堆栈大小
        NULL,                // 任务参数
        1,                   // 任务优先级
        NULL                 // 任务句柄
    );

    xTaskCreate(
        UltrasonicSensor::getDistanceTask,  // 任务函数
        "Sonic_Task",                       // 任务名称
        4096,                               // 堆栈大小
        &ultrasonic,                        // 传入超声波对象指针作为参数
        3,                                  // 任务优先级
        NULL                                // 任务句柄
    );

    LEDController::setMode(LED_BLUE_BLINK);
    
}

void loop() {

    server.handleClient();

    // 检查BLE连接状态的示例
    if (BLEHandler::isConnected()) {
        // 设备已连接时的其他操作
    }
}

