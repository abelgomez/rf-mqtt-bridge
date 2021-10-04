/**
 * Copyright (C) 2021 Abel Gómez
 * 
 * RF-MQTT Brdige is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * RF-MQTT Brdige is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with RF-MQTT Brdige. If not, see <http://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include <PubSubClient.h>
#include <RCSwitch.h>
#include <WiFiClientSecure.h>

#include "GlobalConstants.h"
#include "BridgeWiFiManager.h"
#include "BuiltInLed.h"

BridgeWiFiManager wiFiManager;
RCSwitch emitter;
BuiltInLed builtInLed;
WiFiClientSecure wiFiClient;
PubSubClient mqttClient(wiFiClient);

inline boolean pressed(uint8_t pin) {
    return digitalRead(pin) == LOW;
}

void novyLight() {
    emitter.setProtocol(12);
    emitter.setPulseLength(350);
    emitter.send(BTN_LIGHT);
}

void novyLightOn() {
    emitter.setProtocol(12);
    emitter.setPulseLength(350);
    for (uint i = 0; i < 5; i++) {
        emitter.send(BTN_LIGHT);
        delay(100);
    }
    delay(300);
    emitter.send(BTN_LIGHT);
    delay(400);
    emitter.send(BTN_LIGHT);
}

void novyLightOff() {
    emitter.setProtocol(12);
    emitter.setPulseLength(350);
    for (uint i = 0; i < 5; i++) {
        emitter.send(BTN_LIGHT);
        delay(100);
    }
    delay(300);
    emitter.send(BTN_LIGHT);
}

void novyFanMinus() {
    emitter.setProtocol(12);
    emitter.setPulseLength(350);
    emitter.send(BTN_MINUS);
}

void novyFanPlus() {
    emitter.setProtocol(12);
    emitter.setPulseLength(350);
    emitter.send(BTN_PLUS);
}

void novyFan(uint level) {
    emitter.setProtocol(12);
    emitter.setPulseLength(350);
    emitter.send(BTN_MINUS);
    delay(200);
    emitter.send(BTN_MINUS);
    delay(200);
    emitter.send(BTN_MINUS);
    delay(200);
    emitter.send(BTN_MINUS);
    delay(200);
    for (uint i = 0; i < level; i++) {
        emitter.send(BTN_PLUS);
        delay(200);
    }
}

void plugsOn(uint outlet) {
    emitter.setProtocol(1);
    emitter.setPulseLength(190);
    emitter.send(RC_CODES[outlet-1][0].c_str());
}

void plugsOff(uint outlet) {
    emitter.setProtocol(1);
    emitter.setPulseLength(190);
    emitter.send(RC_CODES[outlet-1][1].c_str());
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    
    String payloadStr;
    for (unsigned int i = 0; i < length; i++) {
        payloadStr += (char) payload[i];
    }
    Serial.println(payloadStr);
    if (TOPIC_NOVY_LIGHT.equals(topic)) {
        Serial.print("Novy light: ");
        Serial.println(payloadStr);
        if (payloadStr.equalsIgnoreCase("on")) {
            novyLightOn();
        } else if (payloadStr.equalsIgnoreCase("off")) {
            novyLightOff();
        } else {
            novyLight();
        }
    } else if (TOPIC_NOVY_MINUS.equals(topic)) {
        Serial.println("Novy fan -");
        novyFanMinus();
    } else if (TOPIC_NOVY_PLUS.equals(topic)) {
        Serial.println("Novy fan +");
        novyFanPlus();
    } else if (TOPIC_NOVY_FAN.equals(topic)) {
        Serial.print("Novy fan level: ");
        Serial.println(payloadStr);
        novyFan(payloadStr.toInt());
    } else if (TOPIC_ETEKCITY_OUTLET_ON.equals(topic)) {
        Serial.print("Outlet on: ");
        Serial.println(payloadStr);
        plugsOn(payloadStr.toInt());
    } else if (TOPIC_ETEKCITY_OUTLET_OFF.equals(topic)) {
        Serial.print("Outlet off: ");
        Serial.println(payloadStr);
        plugsOff(payloadStr.toInt());
    }
}

String mqttStateToString(int state) {
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

boolean reconnect() {
    boolean success = false;

    String broker = wiFiManager.getBroker();
    uint port = wiFiManager.getPort();

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
        Serial.print(mqttStateToString(mqttClient.state()).c_str());
        Serial.println(").");
        success = false;
    }

    builtInLed.off();

    return success;
}

void setup() {
    Serial.begin(9600);
    Serial.println();
    Serial.println("Start setup.");

    wiFiManager.loadConfig();
    wiFiClient.setInsecure();

    if (!wiFiManager.autoConnect()) {
        Serial.println("Unable to connect. Sleeping.");
        ESP.deepSleep(0);
    }

    emitter.enableTransmit(TX_PIN);

    mqttClient.setCallback(callback);
    Serial.println("End setup.");
}

void loop() {
    if (pressed(BUTTON)) {
        Serial.println("Flash button pressed.");
        builtInLed.blink();
        wiFiManager.startConfigPortal();
    }
    if (!mqttClient.connected() && !reconnect()) {
        Serial.println("Sleeping for 5 seconds...");
        delay(5000);
    }
    mqttClient.loop();
}
