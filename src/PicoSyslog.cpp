#include <WiFiUdp.h>

#include "PicoSyslog.h"

namespace PicoSyslog {

class Stream: public SyslogPrint {
    public:
        Stream(const Logger & logger, const LogLevel & level)
            : logger(logger), level(level), packet_in_progress(false) {}

        const Logger & logger;
        const LogLevel level;

        Stream(const Stream &) = delete;
        Stream & operator=(const Stream &) = delete;

        size_t write(const uint8_t * buffer, size_t size) override {
            if (logger.forward_to) {
                logger.forward_to->write(buffer, size);
            }

            if (!logger.server.length() || !logger.port) {
                return size;
            }

            size_t written = 0;
            while (written < size) {
                const size_t left = size - written;
                const uint8_t * pos = (const uint8_t *) memchr(buffer, '\n', left);
                const size_t len = pos ? pos - buffer : left;

                if (!packet_in_progress) {
                    udp.beginPacket(logger.server.c_str(), logger.port);
                    const int priority = (1 << 3) | static_cast<int>(level);
                    udp.printf("<%d> %s %s: ", priority, logger.host.c_str(), logger.app.c_str());
                    packet_in_progress = true;
                }

                udp.write(buffer, len);
                written += len;

                if (!pos) {
                    // separator not found
                    break;
                }

                // separator found, advance in the buffer
                buffer = pos + 1; // skip the newline char too
                size -= len + 1;
                udp.endPacket();
                packet_in_progress = false;
            }
            return written;
        }

        virtual ~Stream() {
            if (packet_in_progress) { udp.endPacket(); }
        }

    protected:
        WiFiUDP udp;
        bool packet_in_progress;
};

Print & Logger::get_stream(const LogLevel log_level) const {
    std::unique_ptr<Print> & stream_ptr = streams[static_cast<int>(log_level)];

    if (!stream_ptr) {
        stream_ptr.reset(new Stream(*this, log_level));
    }

    return * stream_ptr.get();
}

}
