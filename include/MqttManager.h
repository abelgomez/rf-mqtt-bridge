/**
 * Copyright (C) 2021 agomez
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

#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#include "BuiltInLed.h"


class MqttManager {
private:
    BuiltInLed builtInLed;
    WiFiClientSecure wiFiClient;
    PubSubClient mqttClient;
    
    String broker;
    uint port;

    void mqttStateToString(int state);
    boolean reconnect();
    String stateToString(int state);
    void handleMessage(char *topic, uint8_t *bpayload, unsigned int length);
    void handleCommand(String vendor, int device, String command, String payload);
    void handleCommandNovy(int device, String command, String payload);
    void handleCommandEtekcity(int device, String command);

public:
    MqttManager();
    void loop();
    void setConnectionInfo(String broker, uint port);
};