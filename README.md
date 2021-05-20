# AnotherTempSensor

[![CodeFactor](https://www.codefactor.io/repository/github/appdevelopmentandsuch/anothertempsensor/badge)](https://www.codefactor.io/repository/github/appdevelopmentandsuch/anothertempsensor)

Temperature / Humidity Logger using NodeMCU, DHT11, sending data either using MQTT, or pulling directly from device's REST API

## Components

|                                                                   DHT 11                                                                   |                                                         NodeMCU 1.0 (ESP-12 Module)                                                          |
| :----------------------------------------------------------------------------------------------------------------------------------------: | :------------------------------------------------------------------------------------------------------------------------------------------: |
| <img src="https://user-images.githubusercontent.com/22528729/105792674-649c0880-5f4d-11eb-98a0-d07602722df2.jpg" alt="DHT11" width="200"/> | <img src="https://user-images.githubusercontent.com/22528729/105792624-5a7a0a00-5f4d-11eb-8d6b-925c647c68b1.jpg" alt="NodeMCU" width="200"/> |
|             [Buy DHT11 on Amazon (CA)](https://www.amazon.ca/KeeYees-Temperature-Humidity-Single-Bus-Raspberry/dp/B07V5MTQJG/)             |           [Buy NodeMCU 12E on Amazon (CA)](https://www.amazon.ca/KeeYees-Internet-Development-Wireless-Compatible/dp/B07S5Z3VYZ/)            |

## Wiring

<img src="https://user-images.githubusercontent.com/22528729/118985613-356a3680-b944-11eb-891d-28da8cf00c29.png" alt="Wiring" width="200"/>

## Quick Start

1. Download and install Arduino IDE

2. Go to `File -> Preferences` and under `Additional Boards Manager URLs` enter `http://arduino.esp8266.com/stable/package_esp8266com_index.json`

3. Install the necessary libraries by going to `Tools -> Manage Libraries...` and filtering on the libraries below.

```bash
ArduinoJson Version 6.18.0
ArduinoMQTTClient Version 0.1.5
DHT sensor library Version 1.4.1
```

4. Copy `sample.secrets.h`, rename it to `secrets.h` and populate it with Wifi credentials, and either MQTT creds or REST API credentials.

5. Navigate to `Tools -> Boards -> ESP8266 Boards` and select `NodeMCU 1.0 (ESP-12 Module)`.

6. Plug your board into your computer and select the appropriate `Port` in `Tools -> Port`, for example mine was `/dev/ttyUSB0`.

7. You can either `Verify` the sketch, to see if everything compiles without error, or you can `Upload` the sketch, which will compile it anyway and warn you of any issues then.

8. (Debugging) Open the `Serial Monitor` to `115200`, and you should be able to tell if your device connects to Wifi, and if so, what its IP address is.

## Usage

### REST API

You should be able to query the API by opening an API application such as Postman, setting `Authentication` to `Basic Auth`, setting `Username` and `Password` to what you set `SECRET_SERVER_USERNAME` and `SECRET_SERVER_PASSWORD` to, and setting the URL to one of the possible endpoints, i.e. `http://[device_ip]/api/read/all/`, and getting a result back, i.e.

```bash
{
    "temperature": 25.4,
    "humidity": 39,
    "identifier": "##:##:##:##:##:##"
}
```

#### Documentation

**GET** `/api/read/all/`

```bash
{
"temperature": 22.5,
"humidity": 45,
"identifier": "##:##:##:##:##:##"
}
```

**GET** `/api/read/temperature/`

```bash
{
"temperature": 22.5,
"identifier": "##:##:##:##:##:##"
}
```

**GET** `/api/read/humidity/`

```bash
{
"humidity": 45,
"identifier": "##:##:##:##:##:##"
}
```

**GET** `/api/info/`

```bash
{
"identifier": "##:##:##:##:##:##",
"version": "#.#.#"
}
```

**WARNING** The current REST API server setup is not a very secure method of protecting your endpoints, as the username and password are simply base64 encoded and can be read VERY easily as the server is not HTTPS encrypted. This is HOPEFULLY a temporary measure until something sturdier is put in place.

### MQTT

Using an MQTT application such as MQTT Explorer, you should be able to see `temperature` and `humidity` as root topics, with the device's `identifier` as a sub-topic, i.e. `temperature/##:##:##:##:##:##: 25.4`
