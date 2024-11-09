#include "config.h"
#include "led_ctrl.h"


volatile bool SystemCheck::isCrush = false;
volatile float SystemCheck::distance = 0;
UltrasonicSensor* SystemCheck::sonicSensor = nullptr;

extern float distance;
bool beep_flag;
const char* ssid = "AoiSora";
const char* password = "TenmaSaki";

TimerHandle_t wifi_timer;

void initGPIO() {
  Serial.begin(115200);

  pinMode(4, OUTPUT ); 
  digitalWrite(4,HIGH); 

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(CRUSH_PIN, INPUT);
  pinMode(BEEP_PIN, OUTPUT);
  pinMode(MOTOR_A1 , OUTPUT);
  pinMode(MOTOR_A2 , OUTPUT);
  pinMode(MOTOR_B1 , OUTPUT);
  pinMode(MOTOR_B2 , OUTPUT);
  pinMode(RADAR_PIN, INPUT);

  ledcSetup(pwmChannel1, pwmFreq, pwmResolution);
  ledcSetup(pwmChannel2, pwmFreq, pwmResolution);
  ledcSetup(pwmChannel3, pwmFreq, pwmResolution);
  ledcSetup(pwmChannel4, pwmFreq, pwmResolution);

  ledcAttachPin(pwmPin1, pwmChannel1);
  ledcAttachPin(pwmPin2, pwmChannel2);
  ledcAttachPin(pwmPin3, pwmChannel3);
  ledcAttachPin(pwmPin4, pwmChannel4);

  Serial.println("GPIO initialized");
}



void SystemCheck::begin() {
  sonicSensor = new UltrasonicSensor(TRIG_PIN, ECHO_PIN);
  sonicSensor->begin();
  // 创建自检任务
    xTaskCreate(
        selfCheckTask,
        "Check_Task",
        4096,
        NULL,
        2,  // 优先级设置为2
        NULL
    );
}

void SystemCheck::selfCheckTask(void* pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    while(1) {
        // 碰撞检测
        if (digitalRead(CRUSH_PIN) == HIGH) {
            isCrush = true;
            LEDController::setMode(LED_RED_BLINK);
            Serial.println("Crush detected!");
        } else {
            isCrush = false;
        }
        
        // 超声波距离检测
        if (sonicSensor != nullptr) {
            float current_distance = sonicSensor->getDistance();
            if (current_distance >= 0 && current_distance < 400) {
                ::distance = current_distance;

                // 根据距离做出响应
                if (::distance < 5) {  // 距离过近警告
                    LEDController::setMode(LED_RED_BLINK);
                    beep_flag = true;
                    //Serial.printf("Warning: Object too close! Distance: %.2f cm\n", distance);
                } else {
                  beep_flag = false;
                  LEDController::setMode(LED_GREEN_SOLID);
                }
            }
        }

        if(beep_flag){
          digitalWrite(BEEP_PIN, HIGH);
        } else {
          digitalWrite(BEEP_PIN, LOW);
        }
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CHECK_INTERVAL));
    }
}
