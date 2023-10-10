#pragma once

#include <memory>
#include <map>

#include <Arduino.h>
#include <WiFiUdp.h>

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

class AbstractLogger;

class Stream: public SyslogPrint {
    public:
        Stream(const AbstractLogger & logger, const LogLevel & level)
            : logger(logger), level(level), packet_in_progress(false) {}

        const AbstractLogger & logger;
        const LogLevel level;

        Stream(const Stream &) = delete;
        Stream & operator=(const Stream &) = delete;

        size_t write(const uint8_t * buffer, size_t size) override;
        virtual ~Stream();

    protected:
        std::unique_ptr<WiFiUDP> udp;
        bool packet_in_progress;
};

class AbstractLogger: public SyslogPrint {
    public:
        AbstractLogger(const String & app, const String & host, Print * forward_to,
                       const String & server, uint16_t port);

        String app;
        String host;
        Print * forward_to;
        String server;
        uint16_t port;
};

class Logger: public AbstractLogger {
    public:
        Logger(const String & app = "PicoSyslog",
               const String & host = "",
               const LogLevel default_loglevel = LogLevel::information,
               Print * forward_to = &Serial,
               const String & server = "", uint16_t port = 514);

        size_t write(const uint8_t * buffer, size_t size) override;
        Print & get_stream(const LogLevel log_level);

        LogLevel default_loglevel;

        Stream emergency;
        Stream alert;
        Stream critical;
        Stream error;
        Stream warning;
        Stream notification;
        Stream information;
        Stream debug;
};

class SimpleLogger: public AbstractLogger {
    public:
        SimpleLogger(const String & app = "PicoSyslog",
                     const String & host = "",
                     const LogLevel loglevel = LogLevel::information,
                     Print * forward_to = &Serial,
                     const String & server = "", uint16_t port = 514);

        size_t write(const uint8_t * buffer, size_t size) override;

    protected:
        Stream stream;
};

}
