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

class Logger;

class Stream: public SyslogPrint {
    public:
        Stream(const Logger & logger, const LogLevel & level)
            : logger(logger), level(level), packet_in_progress(false) {}

        const Logger & logger;
        const LogLevel level;

        Stream(const Stream &) = delete;
        Stream & operator=(const Stream &) = delete;

        size_t write(const uint8_t * buffer, size_t size) override;
        virtual ~Stream();

    protected:
        std::unique_ptr<WiFiUDP> udp;
        bool packet_in_progress;
};

class Logger: public SyslogPrint {
    public:
        Logger(const String & app = "PicoSyslog",
               const String & host = String(ESP.getChipId(), HEX),
               const LogLevel default_loglevel = LogLevel::information,
               Print * forward_to = &Serial,
               const String & server = "", uint16_t port = 514);

        size_t write(const uint8_t * buffer, size_t size) override;

        Print & get_stream(const LogLevel log_level);

        String app;
        String host;
        LogLevel default_loglevel;
        Print * forward_to;
        String server;
        uint16_t port;

        Stream emergency;
        Stream alert;
        Stream critical;
        Stream error;
        Stream warning;
        Stream notification;
        Stream information;
        Stream debug;
};

}
