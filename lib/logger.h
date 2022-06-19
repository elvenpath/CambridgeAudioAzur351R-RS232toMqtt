#include <Syslog.h>
#include <NTPClient.h>

class Logger
{
private:
    Syslog *syslog;
    NTPClient *timeClient;
    bool serial_log_enabled = false;
    bool syslog_log_enabled = true;
    uint16_t log_level = LOG_DEBUG;

public:
    Logger(Syslog &syslog, NTPClient &timeClient, uint16_t log_level, bool serial_log_enabled = true, bool syslog_log_enabled = true);

    void log(uint16_t pri, const String &message);

    void log(uint16_t pri, const char *message);
};