# AnotherTempSensor

[![CodeFactor](https://www.codefactor.io/repository/github/appdevelopmentandsuch/anothertempsensor/badge)](https://www.codefactor.io/repository/github/appdevelopmentandsuch/anothertempsensor)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://GitHub.com/appdevelopmentandsuch/AnotherTempSensor/graphs/commit-activity)
[![GitHub license](https://img.shields.io/github/license/appdevelopmentandsuch/AnotherTempSensor.svg)](https://github.com/appdevelopmentandsuch/AnotherTempSensor/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/appdevelopmentandsuch/AnotherTempSensor.svg)](https://GitHub.com/appdevelopmentandsuch/AnotherTempSensor/releases/)
[![GitHub tag](https://img.shields.io/github/tag/appdevelopmentandsuch/AnotherTempSensor.svg)](https://GitHub.com/appdevelopmentandsuch/AnotherTempSensor/tags/)
[![Github all releases](https://img.shields.io/github/downloads/appdevelopmentandsuch/AnotherTempSensor/total.svg)](https://GitHub.com/appdevelopmentandsuch/AnotherTempSensor/releases/)
[![GitHub stars](https://img.shields.io/github/stars/appdevelopmentandsuch/AnotherTempSensor.svg?style=social&label=Star&maxAge=2592000)](https://GitHub.com/appdevelopmentandsuch/AnotherTempSensor/stargazers/)
[![GitHub issues](https://img.shields.io/github/issues/appdevelopmentandsuch/AnotherTempSensor.svg)](https://GitHub.com/appdevelopmentandsuch/AnotherTempSensor/issues/)
[![GitHub issues-closed](https://img.shields.io/github/issues-closed/appdevelopmentandsuch/AnotherTempSensor.svg)](https://GitHub.com/appdevelopmentandsuch/AnotherTempSensor/issues?q=is%3Aissue+is%3Aclosed)

Temperature / Humidity Logger using NodeMCU, DHT11, sending data either using MQTT, or pulling directly from device's REST API

## Components

|                                                                   DHT 11                                                                   |                                                         NodeMCU 1.0 (ESP-12 Module)                                                          |
| :----------------------------------------------------------------------------------------------------------------------------------------: | :------------------------------------------------------------------------------------------------------------------------------------------: |
| <img src="https://user-images.githubusercontent.com/22528729/105792674-649c0880-5f4d-11eb-98a0-d07602722df2.jpg" alt="DHT11" width="200"/> | <img src="https://user-images.githubusercontent.com/22528729/105792624-5a7a0a00-5f4d-11eb-8d6b-925c647c68b1.jpg" alt="NodeMCU" width="200"/> |
|             [Buy DHT11 on Amazon (CA)](https://www.amazon.ca/KeeYees-Temperature-Humidity-Single-Bus-Raspberry/dp/B07V5MTQJG/)             |           [Buy NodeMCU 12E on Amazon (CA)](https://www.amazon.ca/KeeYees-Internet-Development-Wireless-Compatible/dp/B07S5Z3VYZ/)            |

## Wiring

<img src="https://user-images.githubusercontent.com/22528729/120116063-f316d580-c14b-11eb-9123-5dba09db7a37.png" alt="Wiring" width="200"/>

## Quick Start

1. Download and install Arduino IDE

2. Go to `File -> Preferences` and under `Additional Boards Manager URLs` enter `http://arduino.esp8266.com/stable/package_esp8266com_index.json`

3. Install the necessary libraries by going to `Tools -> Manage Libraries...` and filtering on the libraries below.

```bash
ArduinoJson Version 6.18.0
ArduinoMQTTClient Version 0.1.5
DHT sensor library Version 1.4.1
```

4. Navigate to `Tools -> Boards -> ESP8266 Boards` and select `NodeMCU 1.0 (ESP-12 Module)`.

5. Plug your board into your computer and select the appropriate `Port` in `Tools -> Port`, for example mine was `/dev/ttyUSB0`.

6. You can either `Verify` the sketch, to see if everything compiles without error, or you can `Upload` the sketch, which will compile it anyway and warn you of any issues then.

7. (Debugging) Open the `Serial Monitor` to `115200`, and you should be able to tell if your device connects to Wifi, and if so, what its IP address is.

## Usage

### Configuration

AnotherTempSensor is designed to be easy to setup and use. Without any prior configuration, you should be able to flash a device and use immediately.

**NOTE** By default, AnotherTempSensor does not display verbose output to the Serial Monitor for security purposes. If you wish to see verbose output of the device, you must got into `constants.h` and change `DEBUG` to `true`.

Once flashed, using a computer with Postman, open up your WiFi settings and find the network `AnotherTempSensor` which you can login to using the **password** `password`.

Once connected to the network `AnotherTempSensor`, in Postman, configure the below JSON body and send:

**POST** `http://192.168.4.1/api/config/`

```bash
Raw JSON Body
{
    "ssid": "", //WiFi SSID to connect to
    "pass": "", //WiFi Password for connection
    "mqttBroker": "", //The IP address or hostname for an MQTT broker
    "mqttPort": 1883, //The port number of the MQTT broker
    "mqttUpdateInterval": 5000, //The interval in which you would like the device to publish data to the MQTT broker, in milliseconds, minimum is 500 or 0.5 seconds
    "mqttUser":"", //A username for a valid MQTT user
    "mqttPass":"", //The password for the valid MQTT user
    "restUser":"", //The username you wish to use going forward to make authenticated requests to the device
    "restPass":"", //The password you wish to use going forward to make authenticated requests to the device
    "service": 1 //1 for REST, 2 for MQTT
}
```

Also valid ways to configure:

**POST** `http://192.168.4.1/api/config/`

```bash
// REST Configuration
Raw JSON Body
{
    "ssid": ""
    "pass": ""
    "restUser":"",
    "restPass":"",
    "service": 1
}
```

**POST** `http://192.168.4.1/api/config/`

```bash
// MQTT Configuration
Raw JSON Body
{
    "ssid": "",
    "pass": "",
    "mqttBroker": "",
    "mqttPort": 1883,
    "mqttUpdateInterval": 5000,
    "mqttUser":"",
    "mqttPass":"",
    "service": 2
}
```

If your device received and processed your configuration, you should receive a response like this:

```bash
// Device Response
{
    "success": true,
    "identifier": "##:##:##:##:##:##"
}
```

Otherwise, you will receive one of a possible few messages, indicating what corrections should be made.

The device will automatically reboot and attempt to connect to WiFi using the `ssid` and `pass` you provided. If it is unable to connect for whatever reason, it will revert to the configure stage again and you will be able to again attempt to configure the endpoint.

#### Reset Device

To clear any previous configuration and restore the default one, press the `Flash` button on your device.

### REST API

You should be able to query the API by opening an API application such as Postman, setting `Authentication` to `Basic Auth`, setting `Username` and `Password` to what you set `restUser` and `restPass` to, and setting the URL to one of the possible endpoints, i.e. `http://[device_ip]/api/read/all/`, and getting a result back, i.e.

```bash
{
    "temperature": 25.4,
    "humidity": 39,
    "identifier": "##:##:##:##:##:##"
}
```

#### Documentation

**GET** `http://[device_ip]/api/read/all/`

```bash
{
"temperature": 22.5,
"humidity": 45,
"identifier": "##:##:##:##:##:##"
}
```

**GET** `http://[device_ip]/api/read/temperature/`

```bash
{
"temperature": 22.5,
"identifier": "##:##:##:##:##:##"
}
```

**GET** `http://[device_ip]/api/read/humidity/`

```bash
{
"humidity": 45,
"identifier": "##:##:##:##:##:##"
}
```

**GET** `http://[device_ip]/api/info/`

```bash
{
"identifier": "##:##:##:##:##:##",
"version": "#.#.#"
}
```

**WARNING** The current REST API server setup is not a very secure method of protecting your endpoints, as the username and password are simply base64 encoded and can be read VERY easily as the server is not HTTPS encrypted. This is HOPEFULLY a temporary measure until something sturdier is put in place.

### MQTT

Using an MQTT application such as MQTT Explorer, you should be able to see `temperature` and `humidity` as root topics, with the device's `identifier` as a sub-topic, i.e. `temperature/##:##:##:##:##:##: 25.4`
