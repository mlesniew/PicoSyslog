#pragma once

#include <memory>
#include <map>
#include <Arduino.h>

namespace PicoSyslog {

enum class LogLevel {
    emergency = 0,
    alert,
    critical,
    error,
    warning,
    notification,
    information,
    debug,
};

class SyslogPrint: public Print {
    public:
        using Print::write;
        size_t write(uint8_t c) override { return write(&c, 1); }
};

class Logger: public SyslogPrint {
    public:
        Logger(const String & app = "PicoSyslog",
               const String & host = String(ESP.getChipId(), HEX),
               const LogLevel default_loglevel = LogLevel::information,
               Print * forward_to = &Serial,
               const String & server = "", uint16_t port = 514):
            app(app), host(host), default_loglevel(default_loglevel), forward_to(forward_to), server(server), port(port) {}

        size_t write(const uint8_t * buffer, size_t size) override { return get_stream(default_loglevel).write(buffer, size); }

        Print & get_stream(const LogLevel log_level) const;

        String app;
        String host;
        LogLevel default_loglevel;
        Print * forward_to;
        String server;
        uint16_t port;

    protected:
        mutable std::unique_ptr<Print> streams[8];
};

}
