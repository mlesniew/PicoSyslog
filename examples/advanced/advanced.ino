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

PicoSyslog::Logger syslog(
    // application name reported to syslog server
    "PicoSyslog",
    // hostname reported to syslog server
    // set to empty string to use IP
    "picosyslog",
    // default log level to use
    PicoSyslog::LogLevel::information,
    // pointer to a Print object to forward the messages to
    // use &Serial, to print to the serial, use nullptr to disable
    &Serial,
    // Syslog server address
    "192.168.1.100",
    // Syslog server port
    514
    );

void setup() {
    Serial.begin(115200);
    Serial.println("Connecting to WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) { delay(100); }
    Serial.print("WiFi connected, IP: ");
    Serial.println(WiFi.localIP());

    // All settings passed to the constructor can be set
    // at a later point too
    syslog.app = "PicoSyslog";
    syslog.host = "picosyslog";
    syslog.forward_to = &Serial;
    syslog.default_loglevel = PicoSyslog::LogLevel::information;
    syslog.server = "192.168.1.100";
    syslog.port = 514;
}

unsigned int counter = 0;

void loop() {
    // Use syslog just like Serial
    syslog.print("PicoSyslog ");
    syslog.println("is awesome!");
    syslog.printf("I've said it %i times!\n", ++counter);
    delay(1000);
}
