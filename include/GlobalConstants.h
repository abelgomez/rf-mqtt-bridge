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

#pragma once

#define PORTAL_NAME "RF-MQTT Bridge"
#define PORTAL_TIMEOUT 120

#define DEFAULT_BROKER "mqtt.example.com"
#define DEFAULT_LISTENING_PORT 8883

#define BASE_CLIENT_ID "rf-mqtt-bridge-"

#define TOPIC_RF_MQTT_BRIDGE "rf-mqtt-bridge"
#define TOPIC_NOVY TOPIC_RF_MQTT_BRIDGE "/novy"
#define TOPIC_NOVY_WILDCARD TOPIC_NOVY "/#"
#define TOPIC_NOVY_LIGHT TOPIC_NOVY "/light"
#define TOPIC_NOVY_MINUS TOPIC_NOVY "/minus"
#define TOPIC_NOVY_PLUS TOPIC_NOVY "/plus"
#define TOPIC_NOVY_POWER TOPIC_NOVY "/power"
#define TOPIC_NOVY_NOVY TOPIC_NOVY "/novy"

#define TX_PIN D1
#define RX_PIN D6
#define BUTTON D3

/**
 * Sniffed codes for minus buttons from the original NOVY remote controller
 * 
 *  Code  1 -> 010101010110
 *  Code  2 -> 100101010110
 *  Code  3 -> 000101010110
 *  Code  4 -> 111001010110
 *  Code  5 -> 011001010110
 *  Code  6 -> 101001010110
 *  Code  7 -> 001001010110
 *  Code  8 -> 110001010110
 *  Code  9 -> 010001010110
 *  Code 10 -> 100001010110
 */

// Programmable code (from 1 to 10), see 4 first bits above
#define DEVICE1 "0101"
#define DEVICE2 "1001"
#define DEVICE3 "0001"
#define DEVICE4 "1110"
#define DEVICE5 "0110"
#define DEVICE6 "1010"
#define DEVICE7 "0010"
#define DEVICE8 "1100"
#define DEVICE9 "0100"
#define DEVICE10 "1000"

// This is the selected device, change to other constant if necessary
#define DEVICE DEVICE1

// Prefix (next 4 bits, not sure what it means, if it is always fixed, or if it is part of the command code)
#define PREFIX "0101"

// Full comamands to be sent (code + prefix + command)
#define BTN_LIGHT DEVICE PREFIX "0111010001"
#define BTN_POWER DEVICE PREFIX "0111010011"
#define BTN_PLUS DEVICE PREFIX "0101"
#define BTN_MINUS DEVICE PREFIX "0110"
