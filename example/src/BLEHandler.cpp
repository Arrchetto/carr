#include "BLEHandler.h"
#include "oled.h"

// 静态成员初始化
NimBLEServer* BLEHandler::pServer = nullptr;
NimBLECharacteristic* BLEHandler::pTxCharacteristic = nullptr;
NimBLECharacteristic* BLEHandler::pRxCharacteristic = nullptr;
bool BLEHandler::deviceConnected = false;
bool BLEHandler::oldDeviceConnected = false;
int BLEHandler::bleFlag = 1;

// 服务和特征UUID
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// 回调类定义
class BLEHandler::MyCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic *pCharacteristic) {
        std::string rxValue = pCharacteristic->getValue();
        if (rxValue.length() > 0) {
            Serial.print("收到数据: ");
            Serial.print(rxValue.c_str());
            updateOLEDDisplay(rxValue.c_str());
            Serial.println();

            if(deviceConnected) {
                pTxCharacteristic->setValue(rxValue);
                pTxCharacteristic->notify();
                Serial.println("数据已回发");
            }
        }
    }
};

class BLEHandler::MyServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer *pServer) {
        deviceConnected = true;
    }

    void onDisconnect(NimBLEServer *pServer) {
        deviceConnected = false;
    }
};

void BLEHandler::init() {
    Serial.begin(115200);
    Serial.println("Starting BLE...");
    bleFlag = 1;
}

void BLEHandler::task() {
    if (bleFlag == 1) {
        NimBLEDevice::init("Exlink");
        pServer = NimBLEDevice::createServer();
        pServer->setCallbacks(new MyServerCallbacks());

        NimBLEService *pService = pServer->createService(SERVICE_UUID);

        pTxCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID_TX,
            NIMBLE_PROPERTY::NOTIFY
        );

        pRxCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID_RX,
            NIMBLE_PROPERTY::WRITE
        );
        pRxCharacteristic->setCallbacks(new MyCallbacks());

        pService->start();
        NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->start();

        Serial.println("BLE initialized");
        bleFlag = 2;
    }

    if (bleFlag == 2) {
        if (!deviceConnected && oldDeviceConnected) {
            delay(500);
            pServer->startAdvertising();
            Serial.println("start advertising");
            oldDeviceConnected = deviceConnected;
        }

        if (deviceConnected && !oldDeviceConnected) {
            oldDeviceConnected = deviceConnected;
        }
    }
}
