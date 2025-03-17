# PicoSyslog

**PicoSyslog** is a lightweight logging library for ESP8266 and ESP32 that
makes it easy to send logs to a [syslog](https://en.wikipedia.org/wiki/Syslog)
server.

![Build](https://github.com/mlesniew/PicoSyslog/actions/workflows/ci.yml/badge.svg) ![License](https://img.shields.io/github/license/mlesniew/PicoSyslog) 

[![arduino-library-badge](https://www.ardu-badge.com/badge/PicoSyslog.svg?)](https://www.ardu-badge.com/PicoSyslog) [![PlatformIO library](https://badges.registry.platformio.org/packages/mlesniew/library/PicoSyslog.svg)](https://registry.platformio.org/libraries/mlesniew/PicoSyslog)

[![ESP8266](https://img.shields.io/badge/ESP-8266-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp8266) [![ESP32](https://img.shields.io/badge/ESP-32-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp32)


## Features

* Lightweight & simple – easy to set up and use.
* Drop-in replacement for `Serial` – same API, just swap Serial for
  a `PicoSyslog::Logger` instance.
* Logs are sent to both the syslog server and `Serial`.
* Supports syslog log levels and tags


## Quickstart

```cpp
#include <Arduino.h>
#include <PicoSyslog.h>

PicoSyslog::Logger syslog("esp");  // Use "esp" as the name/tag for syslog

void setup() {
    // usual setup goes here

    // Make sure Serial is initialized too
    Serial.begin(115200); 

    // Set syslog server address
    syslog.server = "192.168.1.100";
}

void loop() {
    // Works just like Serial!  Messages are also printed to Serial automatically.
    syslog.write("Hello ");
    syslog.writeln("Syslog!");

    int a = 1, b = 2;
    syslog.printf("a = %i, b = %i\n", a, b);

    // Use different log levels for better debugging
    syslog.emergency.println("emergency");
    syslog.alert.println("alert");
    syslog.critical.println("critical");
    syslog.error.println("error");
    syslog.warning.println("warning");
    syslog.notification.println("notification");
    syslog.information.println("information");
    syslog.debug.println("debug");

    delay(1000);
}
```

💡 Want more? Check out the [examples](examples) for more ways to use PicoSyslog!


## Installation

* [Arduino IDE](https://www.ardu-badge.com/PicoSyslog)
* [PlatformIO](https://registry.platformio.org/libraries/mlesniew/PicoSyslog/installation)


## Setting up a syslog server

PicoSyslog works with any syslog server listening on UDP port 514 (e.g. rsyslog).

To spin up a quick syslog server using Docker:

```bash
docker run -p 514:514/udp mlesniew/syslog
```

Logs will be displayed live as they arrive.


## Limitations

* PicoSyslog uses UDP, so some messages may be lost even if WiFi is connected
* Best-effort logging: messages can be dropped if the network is unavailable
  (but logging to the serial console will always work).
* Line-buffered output: logs are only sent when a full line (ending with `\n`)
  is written.
