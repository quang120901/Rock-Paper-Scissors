#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "Credentials.h"

AsyncWebServer server(80);

String processor(const String& var) {
    return "";
}

void connectAP()
{
    Serial.println("Connecting to WiFi..");
    WiFi.begin(ssid, password);
    int cnt = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
        cnt++;

        if (cnt > 20)
            break;
    }
    Serial.println(WiFi.localIP());
}