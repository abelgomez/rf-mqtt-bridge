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

static const String PORTAL_NAME = "RF-MQTT Bridge";
static const uint PORTAL_TIMEOUT = 120;

static const String DEFAULT_BROKER = "mqtt.example.com";
static const ushort DEFAULT_LISTENING_PORT = 8883;

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
static const String TOPIC_ETEKCITY =  TOPIC_RF_MQTT_BRIDGE + "/etekcity";
static const String TOPIC_ETEKCITY_OUTLET_ON = TOPIC_ETEKCITY + "/outlet-on";
static const String TOPIC_ETEKCITY_OUTLET_OFF = TOPIC_ETEKCITY +"/outlet-off";

static const uint8_t TX_PIN = D1;
static const uint8_t RX_PIN = D6;
static const uint8_t BUTTON = D3;

/**
 * Sniffed codes for minus button from the original NOVY remote controller
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

static const String RC_CODES[5][2] = {
	// ON                        //OFF 
	{"010001010101010100110011", "010001010101010100111100"}, /* Outlet 1 */
	{"010001010101010111000011", "010001010101010111001100"}, /* Outlet 2 */
	{"010001010101011100000011", "010001010101011100001100"}, /* Outlet 3 */
	{"010001010101110100000011", "010001010101011100001100"}, /* Outlet 4 */
	{"010001010111010100000011", "010001010111010100001100"}, /* Outlet 5 */
};
