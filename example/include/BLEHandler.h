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
    static bool isConnected() { return deviceConnected; }
};

#endif
