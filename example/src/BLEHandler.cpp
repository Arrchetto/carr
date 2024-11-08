#include "BLEHandler.h"
#include "oled.h"
#include "led_ctrl.h"

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

extern OLED oled;

// 添加指令处理函数
void BLEHandler::processCommand(const std::string& cmd) {
    if (cmd.length() < 5 || cmd[0] != '!') return;  // 基本格式检查

    // 提取指令代码
    std::string code = cmd.substr(1);  // 去掉'!'后的部分
    
    // 处理不同的指令
    //  1
    if (code == "B11:") {

        LEDController::setMode(LED_RED_BLINK);
    }
    else if (code == "B10;") {

        LEDController::setMode(LED_RED_SOLID);
    }
    //  2
    else if (code == "B219") {

        LEDController::setMode(LED_BLUE_BLINK);
    }
    else if (code == "B20:") {

        LEDController::setMode(LED_BLUE_SOLID);
    }
    //  3
    else if (code == "B318") {

        LEDController::setMode(LED_GREEN_BLINK);
    }
    else if (code == "B309") {

        LEDController::setMode(LED_GREEN_SOLID);
    }
    //  4
    else if (code == "B417") {

        LEDController::setMode(LED_WHITE_BLINK);
    }
    else if (code == "B408") {

        LEDController::setMode(LED_WHITE_SOLID);
//        sendData("LED set to green solid");
    }
    //前
    else if (code == "B516") {

        LEDController::setMode(LED_RED_BLINK);
    }
    else if (code == "B507") {

        LEDController::setMode(LED_RED_SOLID);
    }
    //后
    else if (code == "B615") {

        LEDController::setMode(LED_BLUE_BLINK);
    }
    else if (code == "B606") {

        LEDController::setMode(LED_BLUE_SOLID);
    }
    //左
    else if (code == "B714") {

        LEDController::setMode(LED_GREEN_BLINK);
    }
    else if (code == "B705") {

        LEDController::setMode(LED_GREEN_SOLID);
    }
    //右
    else if (code == "B813") {

        LEDController::setMode(LED_WHITE_BLINK);
    }
    else if (code == "B804") {

        LEDController::setMode(LED_WHITE_SOLID);
//        sendData("LED set to green solid");
    }

    // ... 可以添加更多指令 ...
}

// 回调类定义
class BLEHandler::MyCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic *pCharacteristic) {
        std::string rxValue = pCharacteristic->getValue();
        if (rxValue.length() > 0) {
            Serial.print("收到指令: ");
            Serial.println(rxValue.c_str());
            
            // 显示在OLED上
            oled.updateDisplay(rxValue.c_str());

            // 处理指令
            processCommand(rxValue);

            // 如果需要回发原始数据
            if(deviceConnected) {
                pTxCharacteristic->setValue(rxValue);
                pTxCharacteristic->notify();
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

void BLEHandler::BLETask(void *pvParameters) {
    BLEHandler::init(); // 初始化BLE
    while (true) {
        BLEHandler::task(); // 运行BLE任务
        vTaskDelay(100 / portTICK_PERIOD_MS); // 延迟100ms
    }
}

void BLEHandler::sendData(const std::string& data) {
    if (deviceConnected) {
        pTxCharacteristic->setValue(data);
        pTxCharacteristic->notify();
        //Serial.print("发送数据: ");
        //Serial.println(data.c_str());
    } else {
        Serial.println("设备未连接，无法发送数据");
    }
}

