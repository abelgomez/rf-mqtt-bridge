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

#include <Arduino.h>

#include "BridgeWiFiManager.h"
#include "BuiltInLed.h"
#include "EtekcityController.h"
#include "GlobalConstants.h"
#include "MqttManager.h"
#include "NovyController.h"

BridgeWiFiManager wiFiManager;
BuiltInLed builtInLed;
MqttManager mqttManager;

inline boolean pressed(uint8_t pin) {
    return digitalRead(pin) == LOW;
}

void setup() {
    Serial.begin(9600);
    Serial.println();
    Serial.println("Start setup.");

    wiFiManager.loadConfig();
    if (!wiFiManager.autoConnect()) {
        Serial.println("Unable to connect. Sleeping.");
        ESP.deepSleep(0);
    }

    mqttManager.setConnectionInfo(wiFiManager.getBroker(), wiFiManager.getPort());

    Serial.println("End setup.");
}

void loop() {
    if (pressed(BUTTON)) {
        Serial.println("Flash button pressed.");
        builtInLed.blink();
        wiFiManager.startConfigPortal();
        mqttManager.setConnectionInfo(wiFiManager.getBroker(), wiFiManager.getPort());
    }
    mqttManager.loop();
}
