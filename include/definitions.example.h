#define MQTT_SERVER ""
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""

#define HTTP_USER "admin"
#define HTTP_PASSWORD "admin"
#define HTTP_PORT 8083

#define SYSLOG_SERVER ""
#define SYSLOG_PORT 514

#define DEVICE_HOSTNAME "ca_receiver_rs232"

#define APP_NAME "Cambridge Audio RS232 to MQTT"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define LOG_ENABLE_SYSLOG true
#define LOG_ENABLE_SERIAL false

#define LOG_LEVEL LOG_DEBUG

#define UPDATE_SIZE_UNKNOWN 0xFFFFFFFF

const char *command_topic = "receiver/cmnd";
const char *status_topic = "receiver/stat";

const unsigned int baud_rate = 9600;

bool powerState = false; // store the power stat of the device
int8_t volume = -33; // default volume until we know the real one
bool isMuted = false; // store if the device is muted
String selectedInput; // store the selected input type
String sourceType; // store the selected source

String receivedSerialMessage; // store the message received from the device over RS2322

const char *onCommand = "#1,01,1";
const char *offCommand = "#1,01,0";

const char *volumeUpCommand = "#1,02";
const char *volumeDownCommand = "#1,03";

const char *muteCommand = "#1,11,01";
const char *unmuteCommand = "#1,11,00";

const char *inputSelectBDCommand = "#2,01,01";
const char *inputSelectCDCommand = "#2,01,02";
const char *inputSelectVideo2Command = "#2,01,03";
const char *inputSelectAuxCommand = "#2,01,04";
const char *inputSelectTunnerCommand = "#2,01,06";
const char *inputSelectTVARCCommand = "#2,01,09";

const char *sourceTypeAnalogueCommand = "#2,04,00";
const char *sourceTypeCoax1Command = "#2,04,01";
const char *sourceTypeCoax2Command = "#2,04,03";
const char *sourceTypeHDMICommand = "#2,04,02";
const char *sourceTypeOpticalCommand = "#2,04,04";

const char *onOffReply = "#6,01";
const char *onReply = "1";
const char *offReply = "00";

const char *volumeUpReply = "#6,02,";
const char *volumeDownReply = "#6,03,";

const char *muteReply = "#6,11";
const char *muteOnReply = "01";
const char *muteOffReply = "00";

const char *inputSelectedReply = "#7,01";
const char *inputSelectedBDReply = "01";
const char *inputSelectedCDReply = "02";
const char *inputSelectedVideo2Reply = "03";
const char *inputSelectedAuxReply = "04";
const char *inputSelectedTunnerReply = "06";
const char *inputSelectedTVARCReply = "09";

const char *sourceTypeReply = "#7,04";
const char *sourceTypeAnalogueReply = "00";
const char *sourceTypeCoax1Reply = "01";
const char *sourceTypeCoax2Reply = "03";
const char *sourceTypeHDMIReply = "02";
const char *sourceTypeOpticalReply = "04";

const char *commandGroupUnknownError = "#11,01";
const char *commandNumberInGroupUnknownError = "#11,02";
const char *commandDataError = "#11,03";

const char *inputSelectedBDLabel = "BD/DVD";
const char *inputSelectedCDLabel = "CD/Video1";
const char *inputSelectedVideo2Label = "Video2";
const char *inputSelectedAuxLabel = "Aux";
const char *inputSelectedTunnerLabel = "Tunner";
const char *inputSelectedTVARCLabel = "TV ARC";

const char *sourceTypeAnalogueLabel = "Analogue";
const char *sourceTypeCoax1Label = "Coax1";
const char *sourceTypeCoax2Label = "Coax2";
const char *sourceTypeHDMILabel = "HDMI";
const char *sourceTypeOpticalLabel = "Optical";