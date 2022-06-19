#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EspMQTTClient.h>
#include <ArduinoJson.h>
#include <Syslog.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <lib/logger.h>

#include <include/definitions.h>

const int BUFFER_SIZE = JSON_OBJECT_SIZE(32);

WiFiClient espClient;
WiFiUDP udpClient;
Syslog syslog(udpClient, SYSLOG_PROTO_IETF);
Logger logger(syslog, LOG_LEVEL, LOG_ENABLE_SERIAL, LOG_ENABLE_SYSLOG);
static WebServer server(80);

static EspMQTTClient mqttClient(
    WIFI_SSID,
    WIFI_PASSWORD,
    MQTT_SERVER,
    (MQTT_USER == NULL || strlen(MQTT_USER) < 1) ? NULL : MQTT_USER,
    (MQTT_PASSWORD == NULL || strlen(MQTT_PASSWORD) < 1) ? NULL : MQTT_PASSWORD,
    DEVICE_HOSTNAME,
    MQTT_PORT);

void publishHomeAssistantDiscoveryESPConfig() {
  if (!ENABLE_HOMEASSISTANT_DISCOVERY) {
    return;
  }
  String wifiMAC = String(WiFi.macAddress());
  mqttClient.publish((home_assistant_mqtt_prefix + "/sensor/" + DEVICE_HOSTNAME + "/linkquality/config").c_str(), ("{\"~\":\"" + MQTT_DEVICE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " Linkquality\"," +
        + "\"device\": {\"identifiers\":[\"receiver_" + DEVICE_HOSTNAME + "_" + wifiMAC.c_str() + "\"],\"manufacturer\":\"" + manufacturer + "\",\"model\":\"" + model + "\",\"name\": \"" + DEVICE_HOSTNAME + "\" }," +
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_linkquality\"," +
        + "\"stat_t\": \"~\"," +
        + "\"value_template\": \"{{ value_json.rssi }}\"," +
        + "\"icon\":\"mdi:signal\"," +
        + "\"unit_of_meas\": \"rssi\"}").c_str(), true);
}

void initSyslog()
{
  syslog.server(SYSLOG_SERVER, SYSLOG_PORT);
  syslog.deviceHostname(DEVICE_HOSTNAME);
  syslog.defaultPriority(LOG_INFO);

  logger.log(LOG_DEBUG, "Syslog setup finished. Logging...");
}

void sendDeviceStateToMQTT()
{
  StaticJsonDocument<BUFFER_SIZE> jsonDocument;
  StaticJsonDocument<BUFFER_SIZE> jsonStatsDocument;

  jsonDocument["status"] = powerState ? "on" : "off";
  jsonDocument["volume"] = volume;
  jsonDocument["isMute"] = isMuted;
  jsonDocument["selectedInput"] = selectedInput;
  jsonDocument["sourceType"] = sourceType;

  char buffer[BUFFER_SIZE];
  size_t n = serializeJson(jsonDocument, buffer);
  mqttClient.publish(MQTT_STATE_TOPIC.c_str(), buffer, true);
}

void sendEspDeviceStateToMQTT() {
  StaticJsonDocument<BUFFER_SIZE> jsonDocument;
  jsonDocument["ip"] = WiFi.localIP().toString();
  jsonDocument["mac"] = WiFi.macAddress();
  jsonDocument["ssid"] = WiFi.SSID();
  jsonDocument["rssi"] = WiFi.RSSI();

  char buffer[BUFFER_SIZE];
  size_t n = serializeJson(jsonDocument, buffer);

  mqttClient.publish(MQTT_DEVICE_TOPIC.c_str(), buffer, true);
}

void sendCommandToSerial(String command)
{
  logger.log(LOG_DEBUG, "sending command to serial");
  logger.log(LOG_DEBUG, command);

  Serial.println(command);
}

String checkForError(String replyMessage)
{
  if (replyMessage == commandGroupUnknownError)
  {
    return "unknown command group";
  }
  if (replyMessage == commandNumberInGroupUnknownError)
  {
    return "unknown command number in group";
  }
  if (replyMessage == commandDataError)
  {
    return "unknown command data";
  }

  return "";
}

void handleOnOffReply(String onOffReplyMessage)
{
  logger.log(LOG_DEBUG, "On/off reply message: " + onOffReplyMessage);

  if (onOffReplyMessage == onReply) {
    powerState = true;
  } else if (onOffReplyMessage == onReplyAlternative) {
    powerState = true;
  } else if (onOffReplyMessage == offReply) {
    powerState = false;
  } else {
    logger.log(LOG_ERR, "Unknown on/off reply message: " + onOffReplyMessage);
  }
}

void handleVolumeMessage(String volumeReplyMessage)
{
  logger.log(LOG_DEBUG, "Volume reply message: " + volumeReplyMessage);

  volume = volumeReplyMessage.toInt();
  volume = volume ? volume : -33;
}

void handleMuteReply(String muteReplyMessage)
{
  logger.log(LOG_DEBUG, "Mute reply message: " + muteReplyMessage);

  if (muteReplyMessage == muteOnReply)
  {
    isMuted = true;
  }
  else if (muteReplyMessage == muteOffReply)
  {
    isMuted = false;
  }
  else
  {
    logger.log(LOG_ERR, "Unknown mute reply message: " + muteReplyMessage);
  }
}

void handleInputTypeMessage(String inputTypeMessage)
{
  logger.log(LOG_DEBUG, "Input type reply message: " + inputTypeMessage);

  if (inputTypeMessage == inputSelectedCDReply)
  {
    selectedInput = inputSelectedCDLabel;
  }
  else if (inputTypeMessage == inputSelectedVideo2Reply)
  {
    selectedInput = inputSelectedVideo2Label;
  }
  else if (inputTypeMessage == inputSelectedAuxReply)
  {
    selectedInput = inputSelectedAuxLabel;
  }
  else if (inputTypeMessage == inputSelectedTunnerReply)
  {
    selectedInput = inputSelectedTunnerLabel;
  }
  else if (inputTypeMessage == inputSelectedTVARCReply)
  {
    selectedInput = inputSelectedTVARCLabel;
  }
  else
  {
    logger.log(LOG_ERR, "Unknown input type reply message: " + inputTypeMessage);
  }
}

void handleSourceMessage(String sourceMessage)
{
  logger.log(LOG_DEBUG, "Source reply message: " + sourceMessage);

  if (sourceMessage == sourceTypeAnalogueReply)
  {
    sourceType = sourceTypeAnalogueLabel;
  }
  else if (sourceMessage == sourceTypeCoax1Reply)
  {
    sourceType = sourceTypeCoax1Label;
  }
  else if (sourceMessage == sourceTypeCoax2Reply)
  {
    selectedInput = sourceTypeCoax2Label;
  }
  else if (sourceMessage == sourceTypeHDMIReply)
  {
    sourceType = sourceTypeHDMILabel;
  }
  else if (sourceMessage == sourceTypeOpticalReply)
  {
    sourceType = sourceTypeOpticalLabel;
  }
  else
  {
    logger.log(LOG_ERR, "Unknown source reply message: " + sourceMessage);
  }
}

bool handleReceivedMessage(String replyMessage)
{
  String error = checkForError(replyMessage);

  if (!error.isEmpty())
  {
    logger.log(LOG_ERR, "Got error:" + error);

    return false;
  }

  if (replyMessage.substring(0, 5) == onOffReply)
  {
    handleOnOffReply(replyMessage.substring(6));
  }
  else if (replyMessage.substring(0, 5) == muteReply)
  {
    handleMuteReply(replyMessage.substring(6));
  }
  else if (replyMessage.substring(0, 5) == inputSelectedReply)
  {
    handleInputTypeMessage(replyMessage.substring(6));
  }
  else if (replyMessage.substring(0, 5) == sourceTypeReply)
  {
    handleSourceMessage(replyMessage.substring(6));
  }
  else if (replyMessage.substring(0, 6) == volumeUpReply || replyMessage.substring(0, 6) == volumeDownReply) // volume reply
  {
    handleVolumeMessage(replyMessage.substring(6));
  }
  else
  {
    logger.log(LOG_ERR, "Unknown/custom reply message: " + replyMessage);

    return false;
  }

  sendDeviceStateToMQTT();

  return true;
}

void serialReadLoop()
{
  while (Serial.available() > 0)
  {
    char recieved = Serial.read();
    receivedSerialMessage += recieved;

    // Process message when carriage return character is recieved
    if (recieved == '\r')
    {
      receivedSerialMessage.trim();
      if (!receivedSerialMessage.isEmpty())
      {
        logger.log(LOG_DEBUG, "Got message from serial: " + receivedSerialMessage);

        handleReceivedMessage(receivedSerialMessage);

        receivedSerialMessage = ""; // Clear recieved buffer
      }
    }
  }
}

bool processJson(String message)
{
  StaticJsonDocument<BUFFER_SIZE> jsonDocument;

  deserializeJson(jsonDocument, message);
  JsonObject root = jsonDocument.as<JsonObject>();

  if (root.isNull())
  {
    logger.log(LOG_ERR, "parseObject() failed");
    logger.log(LOG_ERR, message);

    return false;
  }

  if (root.containsKey("status"))
  {
    if (root["status"] == "on")
    {
      sendCommandToSerial(onCommand);
    }
    else if (root["status"] == "off")
    {
      sendCommandToSerial(offCommand);

      return true;
    }
  }

  if (root.containsKey("mute"))
  {
    if (root["mute"] == "on")
    {
      sendCommandToSerial(muteCommand);
    }
    else if (root["mute"] == "off")
    {
      sendCommandToSerial(unmuteCommand);
    }
  }

  if (root.containsKey("volume"))
  {
    if (root["volume"] == "up")
    {
      sendCommandToSerial(volumeUpCommand);
    }
    else if (root["volume"] == "down")
    {
      sendCommandToSerial(volumeDownCommand);
    }
  }

  if (root.containsKey("input"))
  {
    if (root["input"] == inputSelectedBDLabel)
    {
      sendCommandToSerial(inputSelectBDCommand);
    }
    else if (root["input"] == inputSelectedCDLabel)
    {
      sendCommandToSerial(inputSelectCDCommand);
    }
    else if (root["input"] == inputSelectedVideo2Label)
    {
      sendCommandToSerial(inputSelectVideo2Command);
    }
    else if (root["input"] == inputSelectedAuxLabel)
    {
      sendCommandToSerial(inputSelectAuxCommand);
    }
    else if (root["input"] == inputSelectedTunnerLabel)
    {
      sendCommandToSerial(inputSelectTunnerCommand);
    }
    else if (root["input"] == inputSelectedTVARCLabel)
    {
      sendCommandToSerial(inputSelectTVARCCommand);
    }
  }

  if (root.containsKey("sourceType"))
  {
    if (root["sourceType"] == sourceTypeAnalogueLabel)
    {
      sendCommandToSerial(sourceTypeAnalogueCommand);
    }
    else if (root["sourceType"] == sourceTypeCoax1Label)
    {
      sendCommandToSerial(sourceTypeCoax1Command);
    }
    else if (root["sourceType"] == sourceTypeCoax2Label)
    {
      sendCommandToSerial(sourceTypeCoax2Command);
    }
    else if (root["input"] == sourceTypeHDMILabel)
    {
      sendCommandToSerial(sourceTypeHDMICommand);
    }
    else if (root["input"] == sourceTypeOpticalLabel)
    {
      sendCommandToSerial(sourceTypeOpticalCommand);
    }
  }

  if (root.containsKey("rawCommand"))
  {
    sendCommandToSerial(root["rawCommand"]);
  }

  return true;
}

void initMqtt()
{
  mqttClient.setMqttReconnectionAttemptDelay(100);
  mqttClient.enableLastWillMessage(MQTT_WILL_TOPIC.c_str(), "offline");
  mqttClient.setKeepAlive(60);
  mqttClient.setMaxPacketSize(MQTT_PACKET_SIZE);
  if (LOG_ENABLE_SERIAL) {
    mqttClient.enableDebuggingMessages();
  }
  mqttClient.enableHTTPWebUpdater(HTTP_USER, HTTP_PASSWORD);
  mqttClient.enableOTA(HTTP_PASSWORD);
}

static long lastOnlinePublished = 0;

void publishLastwillOnline()
{
  if ((millis() - lastOnlinePublished) > 30000)
  {
    if (mqttClient.isConnected())
    {
      mqttClient.publish(MQTT_WILL_TOPIC.c_str(), "online", true);
      lastOnlinePublished = millis();
      sendEspDeviceStateToMQTT();
    }
  }
}

void initMdns()
{
  if (!MDNS.begin(DEVICE_HOSTNAME))
  { // http://esp32.local
    logger.log(LOG_EMERG, "Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
}

void onConnectionEstablished()
{
  initMdns();
  server.close();
  server.begin();
  mqttClient.publish(MQTT_WILL_TOPIC.c_str(), "online", true);
  sendDeviceStateToMQTT();
  sendEspDeviceStateToMQTT();

  mqttClient.subscribe(MQTT_COMMAND_TOPIC.c_str(), [](const String &payload) {
    logger.log(LOG_DEBUG, "Message arrived");
    logger.log(LOG_DEBUG, payload);

    if (!processJson(payload)) {
      logger.log(LOG_ERR, "Could not process message");
    } 
  });

  publishHomeAssistantDiscoveryESPConfig();
}

void setup()
{
  Serial.begin(baud_rate);

  pinMode(LED_BUILTIN, OUTPUT);

  initMdns();

  initSyslog();

  initMqtt();

  logger.log(LOG_DEBUG, "Device started.");
}

void loop()
{
  mqttClient.loop();

  serialReadLoop();

  publishLastwillOnline();
}
