#include <Arduino.h>
#include "BLEHandler.h"
#include "WiFi_Server.h"
#include "oled.h"
#include "mpu6050.h"

OLED oled;

void setup() {
    
    setupWiFi();
    oled.setup();
    setupMPU6050();

    xTaskCreate(
        BLEHandler::BLETask, // 任务函数
        "BLE Task",          // 任务名称
        4096,                // 任务堆栈大小
        NULL,                // 任务参数
        1,                   // 任务优先级
        NULL                 // 任务句柄
    );

    wifi_timer = xTimerCreate("WiFiTimer", pdMS_TO_TICKS(2000), pdTRUE, (void*)0, checkWiFiConnection);

    xTimerStart(wifi_timer, 0);

    
}

void loop() {

    server.handleClient();

    // 检查BLE连接状态的示例
    if (BLEHandler::isConnected()) {
        // 设备已连接时的其他操作
    }
}

