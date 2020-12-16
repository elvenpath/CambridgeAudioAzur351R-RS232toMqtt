#include "logger.h"
#include <SoftwareSerial.h>

Logger::Logger(Syslog &syslog, uint16_t log_level, bool serial_log_enabled, bool syslog_log_enabled)
{
    this->syslog = &syslog;
    this->log_level = log_level;
    this->serial_log_enabled = serial_log_enabled;
    this->syslog_log_enabled = syslog_log_enabled;
}

void Logger::log(uint16_t pri, const String &message)
{
    if (pri <= this->log_level)
    {
        if (this->syslog_log_enabled)
        {
            this->syslog->log(pri, message);
        }

        if (this->serial_log_enabled)
        {
            Serial.println(message);
        }
    }
}

void Logger::log(uint16_t pri, const char *message)
{
    if (pri <= this->log_level)
    {
        if (this->syslog_log_enabled)
        {
            this->syslog->log(pri, message);
        }
        if (this->serial_log_enabled)
        {
            Serial.println(message);
        }
    }
}
