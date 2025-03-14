#include <Arduino.h>
#include <PicoSyslog.h>

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#error "This board is not supported."
#endif

#if __has_include("config.h")
#include "config.h"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "WiFi SSID"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "password"
#endif

PicoSyslog::Logger syslog;

void setup() {
    Serial.begin(115200);
    Serial.println("Connecting to WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) { delay(100); }
    Serial.print("WiFi connected, IP: ");
    Serial.println(WiFi.localIP());

    syslog.server = "192.168.1.100";

    // This log level will be used by default
    syslog.default_loglevel = PicoSyslog::LogLevel::information;
}

void loop() {
    // This one uses the default log level
    syslog.println("default");
    // This uses specific log levels
    syslog.emergency.println("emergency");
    syslog.alert.println("alert");
    syslog.critical.println("critical");
    syslog.error.println("error");
    syslog.warning.println("warning");
    syslog.notification.println("notification");
    syslog.information.println("information");
    syslog.debug.println("debug");
    delay(3000);
}
