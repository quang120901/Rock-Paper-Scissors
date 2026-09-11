#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

extern AsyncWebServer server;

String processor(const String& var);
void connectAP();

#endif