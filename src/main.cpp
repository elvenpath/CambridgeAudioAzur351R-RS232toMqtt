#include <Arduino.h>
#include <EspMQTTClient.h>
#include <ArduinoJson.h>
#include <Syslog.h>
#include <WiFiUdp.h>
#include <lib/logger.h>

#include <include/definitions.h>

const int BUFFER_SIZE = JSON_OBJECT_SIZE(32);

WiFiClient espClient;
WiFiUDP udpClient;
Syslog syslog(udpClient, SYSLOG_PROTO_IETF);
Logger logger(syslog, LOG_LEVEL, LOG_ENABLE_SERIAL, LOG_ENABLE_SYSLOG);

static EspMQTTClient client(
    WIFI_SSID,
    WIFI_PASSWORD,
    MQTT_SERVER,
    (MQTT_USER == NULL || strlen(MQTT_USER) < 1) ? NULL : MQTT_USER,
    (MQTT_PASSWORD == NULL || strlen(MQTT_PASSWORD) < 1) ? NULL : MQTT_PASSWORD,
    DEVICE_HOSTNAME,
    MQTT_PORT);

void publishHomeAssistantDiscoveryPowerConfig(String deviceString) {
  if (!ENABLE_HOMEASSISTANT_DISCOVERY) {
    return;
  }
 
  client.publish((home_assistant_mqtt_prefix + "/switch/" + DEVICE_HOSTNAME + "/power/config").c_str(), ("{\"~\":\"" + MQTT_BASE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " Power\"," +
        + deviceString.c_str() +
        + "\"retain\": true," +
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_power\"," +
        + "\"stat_t\": \"" + MQTT_STATE_TOPIC + "\"," +
        + "\"stat_on\": \"on\"," +
        + "\"stat_off\": \"off\"," +
        + "\"pl_on\": \"{\\\"status\\\": \\\"on\\\"}\"," +
        + "\"pl_off\": \"{\\\"status\\\": \\\"off\\\"}\"," +
        + "\"val_tpl\": \"{{ value_json.status }}\"," +
        + "\"cmd_t\": \"" + MQTT_COMMAND_TOPIC + "\"," +
        + "\"icon\":\"mdi:power\"}").c_str(), true);
}

void publishHomeAssistantDiscoverySourceConfig(String deviceString) {
  if (!ENABLE_HOMEASSISTANT_DISCOVERY) {
    return;
  }
 
  client.publish((home_assistant_mqtt_prefix + "/select/" + DEVICE_HOSTNAME + "/input/config").c_str(), ("{\"~\":\"" + MQTT_BASE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " Input\"," +
        + deviceString.c_str() +
        + "\"ent_cat\": \"config\"," + 
        + "\"retain\": true," +
        + "\"options\": [\"BD/DVD\", \"CD/Video1\", \"Video2\", \"Aux\", \"Tuner\", \"TV ARC\"]," +
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_input\"," +
        + "\"stat_t\": \"" + MQTT_STATE_TOPIC + "\"," +
        + "\"val_tpl\": \"{{ value_json.selectedInput }}\"," +
        + "\"cmd_tpl\": \"{\\\"input\\\": \\\"{{ value }}\\\"}\"," +
        + "\"cmd_t\": \"" + MQTT_COMMAND_TOPIC + "\"," +
        + "\"icon\":\"mdi:video-input-component\"}").c_str(), true);
}

void publishHomeAssistantDiscoverySourceTypeConfig(String deviceString) {
  if (!ENABLE_HOMEASSISTANT_DISCOVERY) {
    return;
  }
  
  client.publish((home_assistant_mqtt_prefix + "/select/" + DEVICE_HOSTNAME + "/inputType/config").c_str(), ("{\"~\":\"" + MQTT_BASE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " Input Type\"," +
        + deviceString.c_str() +
        + "\"ent_cat\": \"config\"," + 
        + "\"retain\": true," +
        + "\"options\": [\"Analogue\", \"Coax1\", \"Coax2\", \"HDMI\", \"Optical\"]," +
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_input_type\"," +
        + "\"stat_t\": \"" + MQTT_STATE_TOPIC + "\"," +
        + "\"val_tpl\": \"{{ value_json.sourceType }}\"," +
        + "\"cmd_tpl\": \"{\\\"sourceType\\\": \\\"{{ value }}\\\"}\"," +
        + "\"cmd_t\": \"" + MQTT_COMMAND_TOPIC + "\"," +
        + "\"icon\":\"mdi:audio-input-xlr\"}").c_str(), true);
}

void publishHomeAssistantDiscoveryRestartButton(String deviceString) {
  if (!ENABLE_HOMEASSISTANT_DISCOVERY) {
    return;
  }
  
  client.publish((home_assistant_mqtt_prefix + "/button/" + DEVICE_HOSTNAME + "/restart/config").c_str(), ("{\"~\":\"" + MQTT_BASE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " Restart\"," +
        + deviceString.c_str() +
        + "\"ent_cat\": \"config\"," + 
        + "\"dev_cla\": \"restart\"," + 
        + "\"retain\": true," +
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_restart\"," +
        + "\"stat_t\": \"" + MQTT_STATE_TOPIC + "\"," +
        + "\"payload_press\": \"{\\\"restart\\\": \\\"true\\\"}\"," +
        + "\"cmd_t\": \"" + MQTT_COMMAND_TOPIC + "\"," +
        + "\"icon\":\"mdi:restart\"}").c_str(), true);
}

void publishHomeAssistantDiscoveryVolume(String deviceString) {
  if (!ENABLE_HOMEASSISTANT_DISCOVERY) {
    return;
  }
  
  client.publish((home_assistant_mqtt_prefix + "/sensor/" + DEVICE_HOSTNAME + "/volume/config").c_str(), ("{\"~\":\"" + MQTT_BASE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " Volume\"," +
        + deviceString.c_str() +
        + "\"ent_cat\": \"config\"," + 
        + "\"retain\": true," +
        + "\"dev_cla\": \"signal_strength\"," + 
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_volume\"," +
        + "\"stat_t\": \"" + MQTT_STATE_TOPIC + "\"," +
        + "\"val_tpl\": \"{{ value_json.volume }}\"," +
        + "\"cmd_t\": \"" + MQTT_COMMAND_TOPIC + "\"," +
        + "\"unit_of_meas\": \"db\"," +
        + "\"icon\":\"mdi:volume-high\"}").c_str(), true);

  client.publish((home_assistant_mqtt_prefix + "/button/" + DEVICE_HOSTNAME + "/volume-up/config").c_str(), ("{\"~\":\"" + MQTT_BASE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " Increase Volume\"," +
        + deviceString.c_str() +
        + "\"ent_cat\": \"config\"," + 
        + "\"retain\": true," +
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_volume_increase\"," +
        + "\"stat_t\": \"" + MQTT_STATE_TOPIC + "\"," +
        + "\"payload_press\": \"{\\\"volume\\\": \\\"up\\\"}\"," +
        + "\"cmd_t\": \"" + MQTT_COMMAND_TOPIC + "\"," +
        + "\"icon\":\"mdi:volume-plus\"}").c_str(), true);
  client.publish((home_assistant_mqtt_prefix + "/button/" + DEVICE_HOSTNAME + "/volume-down/config").c_str(), ("{\"~\":\"" + MQTT_BASE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " Decrease Volume\"," +
        + deviceString.c_str() +
        + "\"ent_cat\": \"config\"," + 
        + "\"retain\": true," +
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_volume_decrease\"," +
        + "\"stat_t\": \"" + MQTT_STATE_TOPIC + "\"," +
        + "\"payload_press\": \"{\\\"volume\\\": \\\"down\\\"}\"," +
        + "\"cmd_t\": \"" + MQTT_COMMAND_TOPIC + "\"," +
        + "\"icon\":\"mdi:volume-minus\"}").c_str(), true);

  client.publish((home_assistant_mqtt_prefix + "/binary_sensor/" + DEVICE_HOSTNAME + "/muted/config").c_str(), ("{\"~\":\"" + MQTT_BASE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " Muted\"," +
        + deviceString.c_str() +
        + "\"ent_cat\": \"config\"," + 
        + "\"dev_cla\": \"sound\"," + 
        + "\"enabled_by_default\": false," + 
        + "\"pl_on\": \"on\"," + 
        + "\"pl_off\": \"off\"," + 
        + "\"retain\": true," +
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_muted\"," +
        + "\"stat_t\": \"" + MQTT_STATE_TOPIC + "\"," +
        + "\"val_tpl\": \"{{ value_json.mute }}\"," +
        + "\"cmd_t\": \"" + MQTT_COMMAND_TOPIC + "\"," +
        + "\"icon\":\"mdi:volume-off\"}").c_str(), true);

  client.publish((home_assistant_mqtt_prefix + "/switch/" + DEVICE_HOSTNAME + "/mute/config").c_str(), ("{\"~\":\"" + MQTT_BASE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " Mute\"," +
        + deviceString.c_str() +
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_mute\"," +
        + "\"ent_cat\": \"config\"," + 
        + "\"dev_cla\": \"switch\"," + 
        + "\"stat_t\": \"" + MQTT_STATE_TOPIC + "\"," +
        + "\"stat_on\": \"on\"," +
        + "\"retain\": true," +
        + "\"stat_off\": \"off\"," +
        + "\"pl_on\": \"{\\\"mute\\\": \\\"on\\\"}\"," +
        + "\"pl_off\": \"{\\\"mute\\\": \\\"off\\\"}\"," +
        + "\"val_tpl\": \"{{ value_json.mute }}\"," +
        + "\"cmd_t\": \"" + MQTT_COMMAND_TOPIC + "\"," +
        + "\"icon\":\"mdi:volume-off\"}").c_str(), true);
}

void publishHomeAssistantDiscoveryESPConfig(String deviceString) {
  if (!ENABLE_HOMEASSISTANT_DISCOVERY) {
    return;
  }
  
  client.publish((home_assistant_mqtt_prefix + "/sensor/" + DEVICE_HOSTNAME + "/linkquality/config").c_str(), ("{\"~\":\"" + MQTT_DEVICE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " Linkquality\"," +
        + deviceString.c_str() +
        + "\"ent_cat\": \"diagnostic\"," +
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_linkquality\"," +
        + "\"stat_t\": \"~\"," +
        + "\"value_template\": \"{{ value_json.rssi }}\"," +
        + "\"icon\":\"mdi:signal\"," +
        + "\"unit_of_meas\": \"rssi\"}").c_str(), true);
  client.publish((home_assistant_mqtt_prefix + "/sensor/" + DEVICE_HOSTNAME + "/ssid/config").c_str(), ("{\"~\":\"" + MQTT_DEVICE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " SSID\"," +
        + deviceString.c_str() +
        + "\"ent_cat\": \"diagnostic\"," +
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_ssid\"," +
        + "\"stat_t\": \"~\"," +
        + "\"value_template\": \"{{ value_json.ssid }}\"," +
        + "\"icon\":\"mdi:access-point-network\"}").c_str(), true);
  client.publish((home_assistant_mqtt_prefix + "/sensor/" + DEVICE_HOSTNAME + "/ip/config").c_str(), ("{\"~\":\"" + MQTT_DEVICE_TOPIC + "\"," +
        + "\"name\":\"" + APP_NAME + " IP\"," +
        + deviceString.c_str() +
        + "\"ent_cat\": \"diagnostic\"," +
        + "\"avty_t\": \"" + MQTT_WILL_TOPIC + "\"," +
        + "\"uniq_id\":\"" + DEVICE_HOSTNAME + "_ip\"," +
        + "\"stat_t\": \"~\"," +
        + "\"value_template\": \"{{ value_json.ip }}\"," +
        + "\"icon\":\"mdi:ip\"}").c_str(), true);
}

void publishHomeAssistantDiscovery() {
  std::string deviceString = "\"device\": {\"ids\":[\"receiver_" + std::string(DEVICE_HOSTNAME) + "_" + String(WiFi.macAddress()).c_str() + "\"]," +
                        +"\"mf\":\"" + manufacturer + "\"," +
                        +"\"mdl\":\"" + model + "\"," +
                        +"\"name\": \"" + DEVICE_HOSTNAME + "\"," + 
                        + "\"configuration_url\": \"http://" + WiFi.localIP().toString().c_str() + "\"},";

  publishHomeAssistantDiscoveryESPConfig(deviceString.c_str());
  publishHomeAssistantDiscoveryPowerConfig(deviceString.c_str());
  publishHomeAssistantDiscoverySourceConfig(deviceString.c_str());
  publishHomeAssistantDiscoverySourceTypeConfig(deviceString.c_str());
  publishHomeAssistantDiscoveryRestartButton(deviceString.c_str());
  publishHomeAssistantDiscoveryVolume(deviceString.c_str());
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
  jsonDocument["mute"] = mute ? "on" : "off";
  jsonDocument["selectedInput"] = selectedInput;
  jsonDocument["sourceType"] = sourceType;

  char buffer[BUFFER_SIZE];
  size_t n = serializeJson(jsonDocument, buffer);
  client.publish(MQTT_STATE_TOPIC.c_str(), buffer, true);
}

void sendEspDeviceStateToMQTT() {
  StaticJsonDocument<BUFFER_SIZE> jsonDocument;
  jsonDocument["ip"] = WiFi.localIP().toString();
  jsonDocument["mac"] = WiFi.macAddress();
  jsonDocument["ssid"] = WiFi.SSID();
  jsonDocument["rssi"] = WiFi.RSSI();

  char buffer[BUFFER_SIZE];
  size_t n = serializeJson(jsonDocument, buffer);

  client.publish(MQTT_DEVICE_TOPIC.c_str(), buffer, true);
}

void sendCommandToSerial(String command)
{
  digitalWrite(LED_BUILTIN, HIGH);

  logger.log(LOG_DEBUG, "Sending command to serial " + command);
  Serial.print(command + "\r");
  delay(500);

  digitalWrite(LED_BUILTIN, LOW);
}

void publishCommandStatus(String error = "")
{
  char aBuffer[200];
  StaticJsonDocument<BUFFER_SIZE> jsonDocument;

  if (!error.isEmpty()) {
    jsonDocument["response"] = "error";
    jsonDocument["error"] = error;
  } else {
    jsonDocument["response"] = "success";
  }
  serializeJson(jsonDocument, aBuffer);

  client.publish(MQTT_COMMAND_RESPONSE_TOPIC.c_str(), aBuffer);
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

bool deviceAttributesAreEmpty() {
  return sourceType.isEmpty() || sourceType == NULL || selectedInput.isEmpty() || selectedInput == NULL;
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
  } else if (onOffReplyMessage == offReplyAlternative) {
    powerState = false;
  } else {
    logger.log(LOG_ERR, "Unknown on/off reply message: " + onOffReplyMessage);
    publishCommandStatus("Unknown on/off reply message: " + onOffReplyMessage);
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
    mute = true;
  }
  else if (muteReplyMessage == muteOffReply)
  {
    mute = false;
  }
  else
  {
    logger.log(LOG_ERR, "Unknown mute reply message: " + muteReplyMessage);
    publishCommandStatus("Unknown mute reply message: " + muteReplyMessage);
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
  else if (inputTypeMessage == inputSelectedTunerReply)
  {
    selectedInput = inputSelectedTunerLabel;
  }
  else if (inputTypeMessage == inputSelectedTVARCReply)
  {
    selectedInput = inputSelectedTVARCLabel;
  }
  else
  {
    logger.log(LOG_ERR, "Unknown input type reply message: " + inputTypeMessage);
    publishCommandStatus("Unknown input type reply message: " + inputTypeMessage);

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
    sourceType = sourceTypeCoax2Label;
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
    publishCommandStatus("Unknown source reply message: " + sourceMessage);
  }
}

void clearLastPublishedError() {
  client.publish(MQTT_COMMAND_RESPONSE_TOPIC.c_str(), "");
}

bool isOnOffReply(String replyMessage) {
  return replyMessage.substring(0, 5) == onOffReply;
}

bool isMuteReply(String replyMessage) {
  return replyMessage.substring(0, 5) == muteReply;
}

bool isInputSelectReply(String replyMessage) {
  return replyMessage.substring(0, 5) == inputSelectedReply;
}

bool isSourceTypeReply(String replyMessage) {
  return replyMessage.substring(0, 5) == sourceTypeReply;
}

bool isVolumeReply(String replyMessage) {
  return replyMessage.substring(0, 6) == volumeUpReply || replyMessage.substring(0, 6) == volumeDownReply;
}

bool handleReceivedMessage(String replyMessage)
{
  String error = checkForError(replyMessage);

  if (!error.isEmpty())
  {
    logger.log(LOG_ERR, "Received error from device: " + error);
    receivedSerialMessageIsError = true;

    publishCommandStatus(error);

    return false;
  }

  clearLastPublishedError();

  if (isOnOffReply(replyMessage))
  {
    handleOnOffReply(replyMessage.substring(6));
  }
  else if (isMuteReply(replyMessage))
  {
    handleMuteReply(replyMessage.substring(6));
  }
  else if (isInputSelectReply(replyMessage))
  {
    handleInputTypeMessage(replyMessage.substring(6));
  }
  else if (isSourceTypeReply(replyMessage))
  {
    handleSourceMessage(replyMessage.substring(6));
  }
  else if (isVolumeReply(replyMessage)) // volume reply
  {
    handleVolumeMessage(replyMessage.substring(6));
  }
  else
  {
    receivedSerialMessageIsError = true;
    logger.log(LOG_ERR, "Unknown/custom reply message: " + replyMessage);
    publishCommandStatus("Unknown/custom reply message: " + replyMessage);

    return false;
  }

  receivedSerialMessageIsError = false;
  sendDeviceStateToMQTT();
  publishCommandStatus();

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

        return;
      }

      logger.log(LOG_DEBUG, "Got empty message from device");
      publishCommandStatus("Got empty message from device");
    }
  }
}

void turnDeviceOn() {
  logger.log(LOG_DEBUG, "Powering on device");
  sendCommandToSerial(onCommand);
  delay(2000); // device needs a few seconds to turn on

  if (receivedSerialMessageIsError) { // we got an error trying to turn on device
    logger.log(LOG_DEBUG, "Power on has error");
    for (size_t i = 5; i < 5; i++) { // retry 5 times
      sendCommandToSerial(onCommand);
      delay(2000);
      if (!receivedSerialMessageIsError) {
        break;
      }
    }
  }
}

void turnDeviceOff() {
  logger.log(LOG_DEBUG, "Powering off device");

  sendCommandToSerial(offCommand);
  delay(4000); // device needs a few seconds to turn on

  if (receivedSerialMessageIsError) { // we got an error trying to turn on device
    logger.log(LOG_DEBUG, "Power off has error");

    for (size_t i = 5; i < 5; i++) { // retry 5 times
      sendCommandToSerial(offCommand);
      delay(4000);
      if (!receivedSerialMessageIsError) {
        break;
      }
    }
  }
}

void muteDevice() {
  sendCommandToSerial(muteCommand);
}

void unmuteDevice() {
  sendCommandToSerial(unmuteCommand);
}

void increaseDeviceVolume() {
  sendCommandToSerial(volumeUpCommand);
}

void decreaseDeviceVolume() {
  sendCommandToSerial(volumeDownCommand);
}

bool processJsonCommand(String message)
{
  StaticJsonDocument<BUFFER_SIZE> jsonDocument;

  deserializeJson(jsonDocument, message);
  JsonObject root = jsonDocument.as<JsonObject>();

  if (root.isNull())
  {
    logger.log(LOG_ERR, "Failed to process json " + message);

    return false;
  }

  if (root.containsKey("status"))
  {
    if (root["status"] == "on")
    {
      turnDeviceOn();

      return true;
    }
    else if (root["status"] == "off")
    {
      turnDeviceOff();

      return true;
    }
  }

  if (root.containsKey("mute"))
  {
    if (root["mute"] == "on")
    {
      muteDevice();
    }
    else if (root["mute"] == "off")
    {
      unmuteDevice();
    }
  }

  if (root.containsKey("volume"))
  {
    if (root["volume"] == "up")
    {
      increaseDeviceVolume();
    }
    else if (root["volume"] == "down")
    {
      decreaseDeviceVolume();
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
    else if (root["input"] == inputSelectedTunerLabel)
    {
      sendCommandToSerial(inputSelectTunerCommand);
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

  if (root.containsKey("restart"))
  {
    logger.log(LOG_DEBUG, "Restarting device");
    delay(500);
    ESP.restart();
  }

  return true;
}

void initMqtt()
{
  client.setMqttReconnectionAttemptDelay(100);
  client.enableLastWillMessage(MQTT_WILL_TOPIC.c_str(), "offline");
  client.setKeepAlive(15);
  client.setMaxPacketSize(MQTT_PACKET_SIZE);
  client.enableMQTTPersistence();
}

static long lastOnlinePublished = 0;

void publishLastwillOnline()
{
  if ((millis() - lastOnlinePublished) > 30000)
  {
    if (client.isConnected())
    {
      client.publish(MQTT_WILL_TOPIC.c_str(), "online", true);
      lastOnlinePublished = millis();
      sendEspDeviceStateToMQTT();
    }
  }
}

void initMdns()
{
  if (!MDNS.begin(DEVICE_HOSTNAME)) { // http://esp32.local
    logger.log(LOG_EMERG, "Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
}

void powerCycleDevice() {
  logger.log(LOG_DEBUG, "Source type or source not set. Power cycling device.");
  bool powerStateStapshot = powerState;
  turnDeviceOn();

  delay(5000); // it takes a few seconds for the device to send all the info once powered on

  if (deviceAttributesAreEmpty()) {
    logger.log(LOG_DEBUG, "Device attributes empty after power on.");

    turnDeviceOff();
    turnDeviceOn();

    // wait until we get all the info from the just powered on device
    int retryCount = 0;
    while (deviceAttributesAreEmpty() && retryCount < 5)
    {
      delay(1000); // device needs a few seconds to turn on
      retryCount++;
    }
  }

  if (powerStateStapshot == false) {
    logger.log(LOG_DEBUG, "Device was off. Turning it off again after power cycle.");

    turnDeviceOff();
  }
}

void onConnectionEstablished()
{
  initMdns();
  client.publish(MQTT_WILL_TOPIC.c_str(), "online", true);

  client.subscribe(MQTT_COMMAND_TOPIC.c_str(), [](const String &payload) {
    logger.log(LOG_DEBUG, "Command arrived " + payload);
    if (payload.isEmpty() || payload == NULL) {
      logger.log(LOG_DEBUG, "Empty or null command. Ignoring.");

      return;
    }
    receivedMqttCommand = payload;

    if (!processJsonCommand(payload)) {
      logger.log(LOG_ERR, "Could not process command " + payload);
    } 
  });

  if (client.getConnectionEstablishedCount() > 1) {
    // only publish first time mqtt is connected, not every time there's a reconnect
    return;
  }

  sendDeviceStateToMQTT();
  sendEspDeviceStateToMQTT();

  clearLastPublishedError();

  publishHomeAssistantDiscovery();
}

void setup() {
  // Serial.swap();
  Serial.begin(9600); // this is what the receiver wants
  // Serial.flush();
  

  pinMode(LED_BUILTIN, OUTPUT);

  // Connect to WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("");

  // Wait for connection
  for (int i = 0; i < 25; i++)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      digitalWrite(LED_BUILTIN, LOW);
      if (LOG_ENABLE_SERIAL) {
        Serial.print(".");
      }
      delay(500);
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  digitalWrite(LED_BUILTIN, LOW);
  if (WiFi.status() != WL_CONNECTED)
  {
    delay(2000);
    if (LOG_ENABLE_SERIAL) {
      Serial.println("Restarting...");
    }
    ESP.restart();
  }
  // initMdns();

  initSyslog();

  initMqtt();

  client.enableHTTPWebUpdater(HTTP_USER, HTTP_PASSWORD);
  client.enableOTA(HTTP_PASSWORD);

  if (LOG_ENABLE_SERIAL) {
    client.enableDebuggingMessages();
  }

  // when ESP is restarted and device is powered on, we don't have the source and source type
  // so we do a power cycle, since the device will send the source and source type on power up
  if (deviceAttributesAreEmpty()) {
    powerCycleDevice();
  }

  logger.log(LOG_DEBUG, "Device started.");
}

void loop()
{
  client.loop();

  serialReadLoop();

  publishLastwillOnline();
}
