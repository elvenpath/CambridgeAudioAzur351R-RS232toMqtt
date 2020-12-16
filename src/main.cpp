#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "PubSubClient.h"
#include <ArduinoJson.h>
#include <Syslog.h>
#include <WiFiUdp.h>
#include <AsyncElegantOTA.h>
#include <lib/logger.h>

#include <include/definitions.h>

const int BUFFER_SIZE = JSON_OBJECT_SIZE(32);

WiFiClient espClient;
PubSubClient mqttClient;
WiFiUDP udpClient;
Syslog syslog(udpClient, SYSLOG_PROTO_IETF);
Logger logger(syslog, LOG_LEVEL, LOG_ENABLE_SERIAL, LOG_ENABLE_SYSLOG);
AsyncWebServer server(HTTP_PORT);

void wifiConnect()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  for (int i = 0; i < 25; i++)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  digitalWrite(LED_BUILTIN, LOW);
  if (WiFi.status() != WL_CONNECTED)
  {
    delay(2000);

    ESP.restart();
  }
}

void initSyslog()
{
  syslog.server(SYSLOG_SERVER, SYSLOG_PORT);
  syslog.deviceHostname(DEVICE_HOSTNAME);
  syslog.appName(APP_NAME);
  syslog.defaultPriority(LOG_INFO);
  
  logger.log(LOG_DEBUG, "Syslog setup finished. Logging...");
}

void sendStateToMQTT()
{
  StaticJsonDocument<BUFFER_SIZE> jsonDocument;
  StaticJsonDocument<BUFFER_SIZE> jsonStatsDocument;

  jsonDocument["status"] = powerState ? "on" : "off";
  jsonDocument["volume"] = volume;
  jsonDocument["isMute"] = isMuted;
  jsonDocument["selectedInput"] = selectedInput;
  jsonDocument["sourceType"] = sourceType;

  jsonStatsDocument["ip"] = WiFi.localIP().toString();
  jsonStatsDocument["mac"] = WiFi.macAddress();
  jsonStatsDocument["ssid"] = WiFi.SSID();
  jsonStatsDocument["rssi"] = WiFi.RSSI();

  jsonDocument["stats"] = jsonStatsDocument;

  char buffer[BUFFER_SIZE];
  size_t n = serializeJson(jsonDocument, buffer);
  mqttClient.publish(status_topic, buffer, true);
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

  if (onOffReplyMessage == onReply)
  {
    powerState = true;
  }
  else if (onOffReplyMessage == offReply)
  {
    powerState = false;
  }
  else
  {
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

  sendStateToMQTT();

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

bool processJson(char *message)
{
  StaticJsonDocument<BUFFER_SIZE> jsonDocument;

  deserializeJson(jsonDocument, message);
  JsonObject root = jsonDocument.as<JsonObject>();

  if (root.isNull())
  {
    logger.log(LOG_ERR, "parseObject() failed");

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

void callback(char *topic, byte *payload, unsigned int length)
{
  logger.log(LOG_DEBUG, "Message arrived");

  char message[length + 1];
  for (unsigned int i = 0; i < length; i++)
  {
    message[i] = (char)payload[i];
  }

  // Conver the incoming byte array to a string
  message[length] = '\0'; // Null terminator used to terminate the char array
  logger.log(LOG_DEBUG, message);

  if (!processJson(message))
  {
    logger.log(LOG_ERR, "Could not process message");

    return;
  }
}

void mqttSetup()
{
  mqttClient.setClient(espClient);
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  logger.log(LOG_INFO, "MQTT client configured");
  mqttClient.setCallback(callback);
}

void mqttConnect()
{
  mqttSetup();

  // Loop until we're reconnected
  while (!mqttClient.connected())
  {
    digitalWrite(LED_BUILTIN, LOW);
    if (mqttClient.connect(DEVICE_HOSTNAME, MQTT_USER, MQTT_PASSWORD))
    {
      logger.log(LOG_INFO, "MQTT connected");
      digitalWrite(LED_BUILTIN, HIGH);
      mqttClient.subscribe(command_topic);
      delay(1000);
    }
    else
    {
      logger.log(LOG_ERR, "MQTT connection failed, rc=" + String(mqttClient.state()));
      logger.log(LOG_ERR, "Trying again in 5 seconds");
      delay(5000);
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
}

void initWebserver()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am here.");
  });

  AsyncElegantOTA.begin(&server, HTTP_USER, HTTP_PASSWORD); // Start ElegantOTA

  server.begin();
  logger.log(LOG_DEBUG, "HTTP server started");
}

void setup()
{
  Serial.begin(baud_rate);

  pinMode(LED_BUILTIN, OUTPUT);

  wifiConnect();

  initWebserver();

  initSyslog();

  mqttConnect();

  sendStateToMQTT();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    wifiConnect();
  }

  AsyncElegantOTA.loop();

  if (!mqttClient.connected())
  {
    logger.log(LOG_ERR, "MQTT not Connected!");
    delay(1000);
    mqttConnect();
  }

  mqttClient.loop();

  serialReadLoop();
}
