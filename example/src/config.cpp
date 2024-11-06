#include "config.h"

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

  ledcSetup(pwmChannel1, pwmFreq, pwmResolution);
  ledcSetup(pwmChannel2, pwmFreq, pwmResolution);
  ledcAttachPin(pwmPin1, pwmChannel1);
  ledcAttachPin(pwmPin2, pwmChannel2);

  Serial.println("GPIO initialized");
}

