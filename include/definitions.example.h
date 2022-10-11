static const char *MQTT_SERVER = "";
static const u_int MQTT_PORT = 1883;
static const char *MQTT_USER = "";
static const char *MQTT_PASSWORD = "";

static const char *HTTP_USER = "admin";
static const char *HTTP_PASSWORD = "admin";

static const char *SYSLOG_SERVER = "";
static const u_int SYSLOG_PORT = 513;

static const char *DEVICE_HOSTNAME = "ca_receiver_rs232";

static const char *WIFI_SSID = "";
static const char *WIFI_PASSWORD = "";

static const bool LOG_ENABLE_SYSLOG = true;
static const bool LOG_ENABLE_SERIAL = false;

static const u_int LOG_LEVEL = LOG_DEBUG;

// ---- stop editing from here down ----

#define UPDATE_SIZE_UNKNOWN 0xFFFFFFFF

static const std::string APP_NAME = "Cambridge Audio RS232 to MQTT";
static const std::string APP_VERSION = "v1.20";

static const std::string MQTT_BASE_TOPIC = "receiver";
static const std::string MQTT_COMMAND_TOPIC = MQTT_BASE_TOPIC + "/cmnd";
static const std::string MQTT_COMMAND_RESPONSE_TOPIC = MQTT_COMMAND_TOPIC + "/response";
static const std::string MQTT_STATE_TOPIC = MQTT_BASE_TOPIC + "/state";
static const std::string MQTT_WILL_TOPIC = MQTT_BASE_TOPIC + "/lastwill";
static const std::string MQTT_DEVICE_TOPIC = MQTT_BASE_TOPIC + "/esp";

static const bool ENABLE_HOMEASSISTANT_DISCOVERY = true;
static const std::string home_assistant_mqtt_prefix = "homeassistant"; 
static const std::string manufacturer = "Cambridge Audio";
static const std::string model = "Azur 351R Receiver";

const char* ntpServer = "";
const long gmtOffsetHours = 3; // GMT +3
unsigned long ntpUpdateHoursInterval = 12;

static const uint16_t MQTT_PACKET_SIZE = 1024;

static const unsigned int baud_rate = 9600;

bool powerState = false; // store the power stat of the device
int8_t volume = -33;     // default volume until we know the real one
bool isMuted = false;    // store if the device is muted
String selectedInput;    // store the selected input type
String sourceType;       // store the selected source

String receivedSerialMessage; // store the message received from the device over RS232
bool receivedSerialMessageIsError = false; // store if the last message received from the device over RS232 has an error
String receivedMqttCommand; // store the mqtt command to be sent to the device over RS232

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
const char *inputSelectTunerCommand = "#2,01,06";
const char *inputSelectTVARCCommand = "#2,01,09";

const char *sourceTypeAnalogueCommand = "#2,04,00";
const char *sourceTypeCoax1Command = "#2,04,01";
const char *sourceTypeCoax2Command = "#2,04,03";
const char *sourceTypeHDMICommand = "#2,04,02";
const char *sourceTypeOpticalCommand = "#2,04,04";

const char *onOffReply = "#6,01";
const char *onReply = "01";
const char *onReplyAlternative = "1"; // for the case the device sends a "1" instead of "01"
const char *offReply = "00";
const char *offReplyAlternative = "0"; // for the case the device sends a "1" instead of "01"

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
const char *inputSelectedTunerReply = "06";
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
const char *inputSelectedTunerLabel = "Tuner";
const char *inputSelectedTVARCLabel = "TV ARC";

const char *sourceTypeAnalogueLabel = "Analogue";
const char *sourceTypeCoax1Label = "Coax1";
const char *sourceTypeCoax2Label = "Coax2";
const char *sourceTypeHDMILabel = "HDMI";
const char *sourceTypeOpticalLabel = "Optical";
