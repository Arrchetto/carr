#include "Wifi_Server.h"
#include "html.h"

bool flag;
bool alarmActive;

bool WIFI_reConnecting = true;  //是否处于连接（重连）中状态，默认begin后自动进入连接状态
bool WIFI_Connected = false;
int reConnectCnt = 0;  // WIFI重连次数
int WIFI_State;        // Declare WIFI_State here

enum WiFiState {
    CONNECTED,
    DISCONNECTED,
    CONNECTING
};

static WiFiState currentState = DISCONNECTED;
static WiFiState previousState = DISCONNECTED; // 添加一个变量来跟踪上一次的连接状态
static unsigned long lastAttemptTime = 0;
static const unsigned long ATTEMPT_INTERVAL = 2000; // 20 seconds between connection attempts

WebServer server(80);

void handleRoot();

void setupWiFi() {

  WiFi.begin(ssid, password);

  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP 服务器已启动");
}

void checkWiFiConnection(TimerHandle_t wifi_timer) {
    unsigned long currentTime = millis();
    WIFI_State = WiFi.status();
    
    switch (currentState) {
        case CONNECTED:
            if (WIFI_State != WL_CONNECTED) {
                currentState = DISCONNECTED;
                alarmActive = true;
            } else {
                alarmActive = false;
            }
            break;
            
        case DISCONNECTED:
            if (currentTime - lastAttemptTime >= ATTEMPT_INTERVAL) {
                WiFi.disconnect();
                WiFi.begin(ssid, password);
                currentState = CONNECTING;
                lastAttemptTime = currentTime;
            }
            alarmActive = true;
            break;
            
        case CONNECTING:
            if (WIFI_State == WL_CONNECTED) {
                currentState = CONNECTED;
                alarmActive = false;
            } else if (currentTime - lastAttemptTime >= ATTEMPT_INTERVAL) {
                currentState = DISCONNECTED;
            }
            break;
    }

    // 仅在连接状态发生变化时输出到串口
    if (currentState != previousState) {
        switch (currentState) {
            case CONNECTED:
                Serial.println("WiFi重新连接成功");
                break;
            case DISCONNECTED:
                Serial.println("WiFi连接已断开");
                break;
            case CONNECTING:
                Serial.println("尝试连接WiFi");
                break;
        }
        previousState = currentState;
    }
}

void handleRoot() {
  server.send(200, "text/html; charset=utf-8", html);
}