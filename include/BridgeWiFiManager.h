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

#pragma once

#include <Arduino.h>
#include <WiFiManager.h>

#include "GlobalConstants.h"

class BridgeWiFiManager {
private:

    WiFiManager wiFiManager;
    WiFiManagerParameter titleLabelParam;
    WiFiManagerParameter brokerParam;
    WiFiManagerParameter portParam;

public:
    BridgeWiFiManager();
    
    String getBroker();
    ushort getPort();
    boolean autoConnect();
    boolean startConfigPortal();
    void loadConfig();
    void saveConfig();
};

