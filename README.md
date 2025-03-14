# PicoSyslog

**PicoSyslog** is a lightweight logging library for ESP8266 and ESP32 that
makes it easy to send logs to a [syslog](https://en.wikipedia.org/wiki/Syslog)
server.


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

TODO


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
