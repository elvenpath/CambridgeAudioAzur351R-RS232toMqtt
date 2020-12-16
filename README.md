
# CambridgeAudioAzur351R-RS232toMqtt

  
Cambridge Audio Azur 351R RS-232 (serial port) to MQTT using and ESP8266 or ESP32.
 

Azur 351R Receiver is a classic receiver without any network/smart functionalities. But it does have a serial port (RS-232) which can be used to send commands and receive response. You can find more information about the serial commands at [Cambrige Audio Azur 351R support page](https://techsupport.cambridgeaudio.com/hc/en-us/articles/200926722-Azur-351R) or directly in the pdf [here](https://techsupport.cambridgeaudio.com/hc/en-us/article_attachments/360000079738/Azur_351R_Serial_Control_Protocol.pdf). In case those pages will not be available in the future, the same file can he found [here](Azur%20351R%20erial%20Control%20Protocol.pdf)
I wanted to have the receiver integrated with my [HomeAssistant](https://www.home-assistant.io/) so i decided to use an ESP8266 and communicate with my HA using MQTT.

#### Instalation
Clone this repository, rename include/definitions.example.h to include/definitions.h, edit it to your needs and compile and write the project an ESP8266/ESP32 with PlatformIO (or whatever else suits you).

#### Commands implemented

 - on/off
 - volume up/down
 - mute/unmute
 - select input source
 - select audio source for input

You can also send raw commands, if you need something else that the ones above

#### MQTT payload examples

    {"status":"on","volume":"up","input":"Aux","source":"Analogue", "rawCommand":"#5,01,"}

    {"rawCommand":"#5,01,"}

    {"volume":"up"}

    {"mute":"on"}

    {"status":"off"}

the response is going to look like this:
`{"status":"on","volume":-56,"isMute":false,"selectedInput":"CD/Video1","sourceType":"Analogue","stats":{"ip":"X.X.X.X","mac":"XX:XX:XX:XX:XX:XX","ssid":"Your_WIFI_SSID","rssi":-76}}`

#### TODO:
- create HomeAssistant custom component
- create 3D printed case for the ESP

##### PS
My C kung fu is not that strong, so please excuse any badly written code. If you think you can improve and want to do so, please open a PR.
