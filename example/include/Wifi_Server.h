#pragma once

#include "config.h"

extern WebServer server;

void setupWiFi();
void setupWebServer();
void handleWebServerClient();
void checkWiFiConnection(TimerHandle_t wifi_timer);

