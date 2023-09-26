#include "PicoSyslog.h"

namespace PicoSyslog {

size_t Stream::write(const uint8_t * buffer, size_t size) {
    if (logger.forward_to) {
        logger.forward_to->write(buffer, size);
    }

    if (!logger.server.length() || !logger.port) {
        return size;
    }

    if (!udp) {
        udp.reset(new WiFiUDP());
    }

    size_t written = 0;
    while (written < size) {
        const size_t left = size - written;
        const uint8_t * pos = (const uint8_t *) memchr(buffer, '\n', left);
        const size_t len = pos ? pos - buffer : left;

        if (!packet_in_progress) {
            udp->beginPacket(logger.server.c_str(), logger.port);
            const int priority = (1 << 3) | static_cast<int>(level);
            udp->printf("<%d> %s %s: ", priority, logger.host.c_str(), logger.app.c_str());
            packet_in_progress = true;
        }

        udp->write(buffer, len);
        written += len;

        if (!pos) {
            // separator not found
            break;
        }

        // separator found, advance in the buffer
        buffer = pos + 1; // skip the newline char too
        size -= len + 1;
        udp->endPacket();
        packet_in_progress = false;
    }
    return written;
}

Stream::~Stream() {
    if (udp && packet_in_progress) { udp->endPacket(); }
}

AbstractLogger::AbstractLogger(const String & app, const String & host,
                               Print * forward_to, const String & server, uint16_t port):
    app(app), host(host), forward_to(forward_to), server(server), port(port) {}

Logger::Logger(const String & app, const String & host, const LogLevel default_loglevel,
               Print * forward_to, const String & server, uint16_t port):
    AbstractLogger(app, host, forward_to, server, port),
    default_loglevel(default_loglevel),
    emergency(*this, LogLevel::emergency),
    alert(*this, LogLevel::alert),
    critical(*this, LogLevel::critical),
    error(*this, LogLevel::error),
    warning(*this, LogLevel::warning),
    notification(*this, LogLevel::notification),
    information(*this, LogLevel::information),
    debug(*this, LogLevel::debug) {}

size_t Logger::write(const uint8_t * buffer, size_t size) {
    return get_stream(default_loglevel).write(buffer, size);
}

Print & Logger::get_stream(const LogLevel level) {
    switch (level) {
        case LogLevel::emergency: return emergency;
        case LogLevel::alert: return alert;
        case LogLevel::critical: return critical;
        case LogLevel::error: return error;
        case LogLevel::warning: return warning;
        case LogLevel::notification: return notification;
        case LogLevel::information: return information;
        case LogLevel::debug: return debug;
        default: return information;
    }
}

SimpleLogger::SimpleLogger(const String & app, const String & host, const LogLevel loglevel,
                           Print * forward_to, const String & server, uint16_t port):
    AbstractLogger(app, host, forward_to, server, port),
    stream(*this, loglevel) {}

size_t SimpleLogger::write(const uint8_t * buffer, size_t size) {
    return stream.write(buffer, size);
}

}
