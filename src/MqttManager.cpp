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

#include "MqttManager.h"
#include "NovyController.h"
#include "EtekcityController.h"

static const String BASE_CLIENT_ID = "rf-mqtt-bridge-";

static const String TOPIC_RF_MQTT_BRIDGE = "rf-mqtt-bridge";
static const String TOPIC_RF_MQTT_BRIDGE_WIDLCARD = TOPIC_RF_MQTT_BRIDGE + "/#";
static const String TOPIC_NOVY = TOPIC_RF_MQTT_BRIDGE + "/novy";
static const String TOPIC_NOVY_LIGHT = TOPIC_NOVY + "/light";
static const String TOPIC_NOVY_MINUS = TOPIC_NOVY + "/minus";
static const String TOPIC_NOVY_PLUS = TOPIC_NOVY + "/plus";
static const String TOPIC_NOVY_POWER = TOPIC_NOVY + "/power";
static const String TOPIC_NOVY_NOVY = TOPIC_NOVY + "/novy";
static const String TOPIC_NOVY_FAN = TOPIC_NOVY + "/fan";
static const String TOPIC_ETEKCITY = TOPIC_RF_MQTT_BRIDGE + "/etekcity";
static const String TOPIC_ETEKCITY_OUTLET_ON = TOPIC_ETEKCITY + "/outlet-on";
static const String TOPIC_ETEKCITY_OUTLET_OFF = TOPIC_ETEKCITY + "/outlet-off";


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
        mqttClient.subscribe(TOPIC_RF_MQTT_BRIDGE_WIDLCARD.c_str());
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


void MqttManager::handleMessage(char *topic, uint8_t *payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    String payloadStr;
    for (unsigned int i = 0; i < length; i++) {
        payloadStr += (char)payload[i];
    }
    Serial.println(payloadStr);
    if (TOPIC_NOVY_LIGHT.equals(topic)) {
        Serial.print("Novy light: ");
        Serial.println(payloadStr);
        NovyController novyController(TX_PIN, 1);
        if (payloadStr.equalsIgnoreCase("on")) {
            novyController.lightOn();
        } else if (payloadStr.equalsIgnoreCase("off")) {
            novyController.lightOff();
        } else {
            novyController.pressLight();
        }
    } else if (TOPIC_NOVY_MINUS.equals(topic)) {
        Serial.println("Novy fan -");
        NovyController novyController(TX_PIN, 1);
        novyController.pressMinus();
    } else if (TOPIC_NOVY_PLUS.equals(topic)) {
        Serial.println("Novy fan +");
        NovyController novyController(TX_PIN, 1);
        novyController.pressPlus();
    } else if (TOPIC_NOVY_FAN.equals(topic)) {
        Serial.print("Novy fan level: ");
        Serial.println(payloadStr);
        NovyController novyController(TX_PIN, 1);
        novyController.fan(payloadStr.toInt());
    } else if (TOPIC_ETEKCITY_OUTLET_ON.equals(topic)) {
        Serial.print("Outlet on: ");
        Serial.println(payloadStr);
        EtekcityController etekcityController(TX_PIN, payloadStr.toInt());
        etekcityController.turnOn();
    } else if (TOPIC_ETEKCITY_OUTLET_OFF.equals(topic)) {
        Serial.print("Outlet off: ");
        Serial.println(payloadStr);
        EtekcityController etekcityController(TX_PIN, payloadStr.toInt());
        etekcityController.turnOff();
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