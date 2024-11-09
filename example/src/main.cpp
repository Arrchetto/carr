#include <Arduino.h>
#include "BLEHandler.h"
#include "WiFi_Server.h"
#include "oled.h"
#include "mpu6050.h"
#include "sonic.h"
#include "led_ctrl.h"
#include "config.h"

#define wifi_flag 0
#define ble_flag 0
#define mpu_flag 1
#define oled_flag 0
#define Sonic_flag 1

float distance;

OLED oled;

void setup() {
    initGPIO();

    LEDController::begin();
    
#if wifi_flag
    setupWiFi();
    wifi_timer = xTimerCreate("WiFiTimer", pdMS_TO_TICKS(2000), pdTRUE, (void*)0, checkWiFiConnection);
    xTimerStart(wifi_timer, 0);
#endif

    // 初始化系统自检（包含超声波检测）
//    SystemCheck::begin();

//    oled.setup();

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

    LEDController::setMode(LED_GREEN_SOLID);
}

void loop() {

    server.handleClient();
}

