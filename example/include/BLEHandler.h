#ifndef BLE_HANDLER_H
#define BLE_HANDLER_H

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLEUtils.h>
#include <NimBLECharacteristic.h>

class BLEHandler {
private:
    static NimBLEServer *pServer;
    static NimBLECharacteristic *pTxCharacteristic;
    static NimBLECharacteristic *pRxCharacteristic;
    static bool deviceConnected;
    static bool oldDeviceConnected;
    static int bleFlag;

    class MyCallbacks;
    class MyServerCallbacks;

public:
    static void init();
    static void task();
    static void BLETask(void *pvParameters);
    static bool isConnected() { return deviceConnected; }
    static void sendData(const std::string& data); // 声明发送数据的函数
    static void processCommand(const std::string& cmd);  // 添加指令处理函数声明
};

#endif
