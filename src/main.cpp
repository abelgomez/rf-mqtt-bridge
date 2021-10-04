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
#include <ESP_EEPROM.h>
#include <PubSubClient.h>
#include <RCSwitch.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>

#include "GlobalConstants.h"

#define MAGIC "MG"
#define MAGIC_LENGTH 2
#define BROKER_LENGTH 64

RCSwitch emitter;
WiFiManager wiFiManager;
WiFiClientSecure wiFiClient;
PubSubClient client(wiFiClient);

char magic[MAGIC_LENGTH] = "";
char broker[BROKER_LENGTH] = DEFAULT_BROKER;
unsigned short port = DEFAULT_LISTENING_PORT;

WiFiManagerParameter titleLabelParam("<h3>Broker configuration</h3><p><small>Values below will only be saved if the above settings are set and a successful connection is established.</small></p>");
WiFiManagerParameter brokerLabelParam("Hostname");
WiFiManagerParameter brokerParam("broker", "Broker hostname", broker, BROKER_LENGTH - 1);
WiFiManagerParameter portLabelParam("Port");
WiFiManagerParameter portParam("port", "Port", String(port).c_str(), 5);

inline boolean pressed(uint8_t pin) {
    return digitalRead(pin) == LOW;
}

void builtInLedOn() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void builtInLedOff() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void blinkBuiltinLed() {
    for (unsigned int i = 0; i < 10; i++) {
        builtInLedOn();
        delay(200);
        builtInLedOff();
        delay(100);
    }
}

boolean loadConfig() {
    EEPROM.begin(MAGIC_LENGTH + BROKER_LENGTH + sizeof(port));
    EEPROM.get(0, magic);
    boolean success = false;
    if (!strcmp(MAGIC, magic)) {
        success = true;
        EEPROM.get(MAGIC_LENGTH, broker);
        EEPROM.get(MAGIC_LENGTH + BROKER_LENGTH, port);
        Serial.print("Loaded broker: ");
        Serial.println(broker);
        Serial.print("Loaded port: ");
        Serial.println(port);
    } else {
        Serial.println("No magic bytes found. Not loading config.");
    }
    EEPROM.end();
    return success;
}

void saveConfig() {
    strncpy(broker, brokerParam.getValue(), BROKER_LENGTH);
    port = (short)atoi(portParam.getValue());

    EEPROM.begin(MAGIC_LENGTH + BROKER_LENGTH + sizeof(port));
    EEPROM.put(0, MAGIC);
    EEPROM.put(MAGIC_LENGTH, broker);
    EEPROM.put(MAGIC_LENGTH + BROKER_LENGTH, port);
    EEPROM.end();

    Serial.print("Saved broker: ");
    Serial.println(broker);
    Serial.print("Saved port: ");
    Serial.println(port);
}

void configureWiFiManager() {
    wiFiManager.addParameter(&titleLabelParam);
    wiFiManager.addParameter(&brokerLabelParam);
    wiFiManager.addParameter(&brokerParam);
    wiFiManager.addParameter(&portLabelParam);
    wiFiManager.addParameter(&portParam);
    wiFiManager.setConfigPortalTimeout(PORTAL_TIMEOUT);
    wiFiManager.setSaveConfigCallback(saveConfig);
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    if (!strncmp(topic, TOPIC_NOVY_LIGHT, sizeof(TOPIC_NOVY_LIGHT))) {
        Serial.println("Novy light on/off");
        emitter.send(BTN_LIGHT);
    }
}

boolean reconnect() {
    boolean success = false;
    Serial.print("Attempting MQTT connection... ");
    builtInLedOn();
    String clientId = BASE_CLIENT_ID;
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
        Serial.println("Connected.");
        client.subscribe(TOPIC_NOVY_WILDCARD);
        success = true;
    } else {
        Serial.print("Failed (");
        switch (client.state()) {
        case MQTT_CONNECTION_TIMEOUT:
            Serial.print("the server didn't respond within the keepalive time");
            break;
        case MQTT_CONNECTION_LOST:
            Serial.print("the network connection was broken");
            break;
        case MQTT_CONNECT_FAILED:
            Serial.print("the network connection failed");
            break;
        case MQTT_DISCONNECTED:
            Serial.print("the client is disconnected cleanly");
            break;
        case MQTT_CONNECTED:
            Serial.print("the client is connected");
            break;
        case MQTT_CONNECT_BAD_PROTOCOL:
            Serial.print("the server doesn't support the requested version of MQTT");
            break;
        case MQTT_CONNECT_BAD_CLIENT_ID:
            Serial.print("the server rejected the client identifier");
            break;
        case MQTT_CONNECT_UNAVAILABLE:
            Serial.print("the server was unable to accept the connection");
            break;
        case MQTT_CONNECT_BAD_CREDENTIALS:
            Serial.print("the username/password were rejected");
            break;
        case MQTT_CONNECT_UNAUTHORIZED:
            Serial.print("the client was not authorized to connect");
            break;
        default:
            Serial.print("unknown");
        }
        Serial.println(").");
        success = false;
    }
    builtInLedOff();
    return success;
}

void setup() {
    Serial.begin(9600);
    Serial.println();

    loadConfig();

    configureWiFiManager();

    if (!wiFiManager.autoConnect(PORTAL_NAME)) {
        Serial.println("Unable to connect. Sleeping.");
        ESP.deepSleep(0);
    }

    emitter.enableTransmit(TX_PIN);
    emitter.setProtocol(12);
    emitter.setPulseLength(350);

    wiFiClient.setInsecure();
    client.setServer(broker, port);
    client.setCallback(callback);
}

void loop() {
    if (pressed(BUTTON)) {
        Serial.println("Flash button pressed.");
        blinkBuiltinLed();
        wiFiManager.startConfigPortal(PORTAL_NAME);
    }
    if (!client.connected() && !reconnect()) {
        Serial.println("Sleeping for 5 seconds...");
        delay(5000);
    }
    client.loop();
}
