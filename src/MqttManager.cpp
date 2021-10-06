/**
 * Copyright (C) 2021 Abel Gómez
 * 
 * RF-MQTT Bridge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * RF-MQTT Bridge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with RF-MQTT Bridge. If not, see <http://www.gnu.org/licenses/>.
 */

#include <Regexp.h>

#include "EtekcityController.h"
#include "MqttManager.h"
#include "NovyController.h"

static const String BASE_CLIENT_ID = "rf-mqtt-bridge-";

/**
 * Topic structure:
 * 
 * rf-mqtt-bridge/<vendor>/<device>/<command>
 * 
 * E.g.:
 * rf-mqtt-bridge/novy/1/light
 * rf-mqtt-bridge/novy/1/fan (In this case, the payload will contain the fan level)
 * rf-mqtt-bridge/etekcity/2/on
 */

static const String BASE_TOPIC = "rf-mqtt-bridge";
static const String BASE_TOPIC_WILDCARD = BASE_TOPIC + "/#";
static const String VENDOR_NOVY = "novy";
static const String VENDOR_ETEKCITY = "etekcity";

static const String COMMAND_LIGHT = "light";
static const String COMMAND_MINUS = "minus";
static const String COMMAND_PLUS = "plus";
static const String COMMAND_POWER = "power";
static const String COMMAND_NOVY = "novy";
static const String COMMAND_FAN = "fan";
static const String COMMAND_ON = "on";
static const String COMMAND_OFF = "off";

MqttManager::MqttManager() : mqttClient(wiFiClient) {
    wiFiClient.setInsecure();
    mqttClient.setCallback(std::bind(&MqttManager::handleMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void MqttManager::setConnectionInfo(String broker, uint port) {
    this->broker = broker;
    this->port = port;
}

void MqttManager::loop() {
    if (!mqttClient.connected() && !reconnect()) {
        Serial.println("Sleeping for 5 seconds...");
        delay(5000);
    }
    mqttClient.loop();
}

boolean MqttManager::reconnect() {
    boolean success = false;

    Serial.print("Attempting MQTT connection to ");
    Serial.print(broker);
    Serial.print(":");
    Serial.print(port);
    Serial.print("... ");

    if (mqttClient.connected()) {
        Serial.print("Already connected. Doing nothing.");
        return true;
    }

    builtInLed.on();

    mqttClient.setServer(broker.c_str(), port);

    String clientId = BASE_CLIENT_ID + String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        mqttClient.subscribe(BASE_TOPIC_WILDCARD.c_str());
        success = true;
    } else {
        Serial.print("Failed (");
        Serial.print(stateToString(mqttClient.state()).c_str());
        Serial.println(").");
        success = false;
    }

    builtInLed.off();

    return success;
}

void MqttManager::handleMessage(char *topic, uint8_t *bpayload, unsigned int length) {
    String payload;
    for (unsigned int i = 0; i < length; i++) {
        payload += (char)bpayload[i];
    }
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: '");
    Serial.print(payload);
    Serial.println("'");

    MatchState ms;
    ms.Target(topic);
    if (ms.Match("(%a+)/(%a+)/(%d+)/(%a+)")) {
        char buffer[128];
        String vendor = String(ms.GetCapture(buffer, 1));
        int device = String(ms.GetCapture(buffer, 2)).toInt();
        String command = String(ms.GetCapture(buffer, 3));
        handleCommand(vendor, device, command, payload);
    } else {
        Serial.print("Unknown topic: ");
        Serial.println(topic);
    }
}

void MqttManager::handleCommand(String vendor, int device, String command, String payload) {
    Serial.print("Handling command '");
    Serial.print(command);
    Serial.print("' for device '");
    Serial.print(device);
    Serial.print("' from vendor '");
    Serial.print(vendor);
    Serial.println("'");

    if (VENDOR_NOVY.equals(vendor)) {
        handleCommandNovy(device, command, payload);
    } else if (VENDOR_ETEKCITY.equals(vendor)) {
        handleCommandEtekcity(device, command);
    } else {
        Serial.print("Unknown vendor: ");
        Serial.println(vendor);
    }
}

void MqttManager::handleCommandNovy(int device, String command, String payload) {
    NovyController novyController(TX_PIN, device);
    if (COMMAND_LIGHT.equals(command)) {
        if (COMMAND_ON.equalsIgnoreCase(payload)) {
            novyController.lightOn();
        } else if (COMMAND_OFF.equalsIgnoreCase(payload)) {
            novyController.lightOff();
        } else {
            novyController.pressLight();
        }
    } else if (COMMAND_MINUS.equals(command)) {
        novyController.pressMinus();
    } else if (COMMAND_PLUS.equals(command)) {
        novyController.pressPlus();
    } else if (COMMAND_FAN.equals(command)) {
        novyController.fan(payload.toInt());
    } else {
        Serial.print("Unknown command: ");
        Serial.println(command);
    }
}
void MqttManager::handleCommandEtekcity(int device, String command) {
    EtekcityController etekcityController(TX_PIN, device);
    if (COMMAND_ON.equals(command)) {
        etekcityController.turnOn();
    } else if (COMMAND_OFF.equals(command)) {
        etekcityController.turnOff();
    } else {
        Serial.print("Unknown command: ");
        Serial.println(command);
    }
}

String MqttManager::stateToString(int state) {
    switch (state) {
    case MQTT_CONNECTION_TIMEOUT:
        return "the server didn't respond within the keepalive time";
    case MQTT_CONNECTION_LOST:
        return "the network connection was broken";
    case MQTT_CONNECT_FAILED:
        return "the network connection failed";
    case MQTT_DISCONNECTED:
        return "the client is disconnected cleanly";
    case MQTT_CONNECTED:
        return "the client is connected";
    case MQTT_CONNECT_BAD_PROTOCOL:
        return "the server doesn't support the requested version of MQTT";
    case MQTT_CONNECT_BAD_CLIENT_ID:
        return "the server rejected the client identifier";
    case MQTT_CONNECT_UNAVAILABLE:
        return "the server was unable to accept the connection";
    case MQTT_CONNECT_BAD_CREDENTIALS:
        return "the username/password were rejected";
    case MQTT_CONNECT_UNAUTHORIZED:
        return "the client was not authorized to connect";
    default:
        return "unknown";
    }
}