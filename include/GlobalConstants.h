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
static const String TOPIC_ETEKCITY = TOPIC_RF_MQTT_BRIDGE + "/etekcity";
static const String TOPIC_ETEKCITY_OUTLET_ON = TOPIC_ETEKCITY + "/outlet-on";
static const String TOPIC_ETEKCITY_OUTLET_OFF = TOPIC_ETEKCITY + "/outlet-off";

static const uint8_t TX_PIN = D1;
static const uint8_t RX_PIN = D6;
static const uint8_t BUTTON = D3;
