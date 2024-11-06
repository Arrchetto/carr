#include <Arduino.h>
#include "BLEHandler.h"
#include "WiFi_Server.h"
#include "oled.h"

void setup() {
    BLEHandler::init();

    setupWiFi();
    setupOLED();

    wifi_timer = xTimerCreate("WiFiTimer", pdMS_TO_TICKS(2000), pdTRUE, (void*)0, checkWiFiConnection);

    xTimerStart(wifi_timer, 0);
}

void loop() {

    server.handleClient();

    BLEHandler::task();

    // 检查BLE连接状态的示例
    if (BLEHandler::isConnected()) {
        // 设备已连接时的其他操作
    }
}

