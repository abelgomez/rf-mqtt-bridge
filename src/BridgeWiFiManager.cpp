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

#include "BridgeWiFiManager.h"

#include <ESP_EEPROM.h>

#define MAGIC "MG"
#define MAGIC_LENGTH 2
#define BROKER_LENGTH 64

static const String PORTAL_NAME = "RF-MQTT Bridge";
static const uint PORTAL_TIMEOUT = 120;

static const String DEFAULT_BROKER = "mqtt.example.com";
static const ushort DEFAULT_LISTENING_PORT = 8883;

BridgeWiFiManager::BridgeWiFiManager() :
        titleLabelParam("<h3>Broker configuration</h3>"),
        brokerParam("broker", "Broker hostname"),
        portParam("port", "Port") 
    {
    wiFiManager.addParameter(&titleLabelParam);
    wiFiManager.addParameter(&brokerParam);
    wiFiManager.addParameter(&portParam);
    wiFiManager.setConfigPortalTimeout(PORTAL_TIMEOUT);
    wiFiManager.setSaveParamsCallback(std::bind(&BridgeWiFiManager::saveConfig, this));
    // Force WiFi off until explicitly connected to avoid showing the ESP SSID even when connected to an AP
    WiFi.mode(WIFI_OFF);
}

String BridgeWiFiManager::getBroker() {
    return brokerParam.getValue();
}

ushort BridgeWiFiManager::getPort() {
    return atoi(portParam.getValue());
}

boolean BridgeWiFiManager::autoConnect() {
    return wiFiManager.autoConnect(PORTAL_NAME.c_str());
}

boolean BridgeWiFiManager::startConfigPortal() {
    return wiFiManager.startConfigPortal(PORTAL_NAME.c_str());
}

void BridgeWiFiManager::loadConfig() {
    Serial.print("Loading config... ");

    char magic[MAGIC_LENGTH];
    char broker[BROKER_LENGTH];
    ushort port;

    EEPROM.begin(MAGIC_LENGTH + BROKER_LENGTH + sizeof(ushort));
    
    EEPROM.get(0, magic);
    if (!strncmp(magic, MAGIC, MAGIC_LENGTH)) {
        Serial.println("Magic bytes found.");
        EEPROM.get(MAGIC_LENGTH, broker);
        EEPROM.get(MAGIC_LENGTH + BROKER_LENGTH, port);
        Serial.print("Loaded broker: ");
        Serial.println(broker);
        Serial.print("Loaded port: ");
        Serial.println(port);
        brokerParam.setValue(broker, BROKER_LENGTH - 1);
        portParam.setValue(String(port).c_str(), 5);
    } else {
        Serial.println("No magic bytes found. Not loading config.");
        brokerParam.setValue(DEFAULT_BROKER.c_str(), BROKER_LENGTH - 1);
        portParam.setValue(String(DEFAULT_LISTENING_PORT).c_str(), 5);
    }
    EEPROM.end();
}

void BridgeWiFiManager::saveConfig() {
    Serial.println("Saving config...");

    Serial.print("Saving broker: ");
    Serial.println(brokerParam.getValue());
    Serial.print("Saving port: ");
    Serial.println(portParam.getValue());

    char broker[BROKER_LENGTH];
    strncpy(broker, brokerParam.getValue(), BROKER_LENGTH);

    EEPROM.begin(MAGIC_LENGTH + BROKER_LENGTH + sizeof(ushort));
    EEPROM.put(0, MAGIC);
    EEPROM.put(MAGIC_LENGTH, broker);
    EEPROM.put(MAGIC_LENGTH + BROKER_LENGTH, (ushort)atoi(portParam.getValue()));
    EEPROM.end();
}
