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

#include "EtekcityController.h"

/**
 * Etekcity constants
 */ 

// Prefix (not sure what it means, if it is always fixed, or if it is part of the device code)
static const String ETEKCITY_PREFIX = "0100010101";
static const String ETEKCITY_DEVICE_CODE[] = {
    "0101010011",
    "0101011100",
    "0101110000",
    "0111010000",
    "1101010000",
};
static const String ETEKCITY_COMMAND_ON = "0011";
static const String ETEKCITY_COMMAND_OFF = "1100";


EtekcityController::EtekcityController(uint8_t pin, ushort device) : pin(pin), device(device) {
    emitter.enableTransmit(pin);
    emitter.setProtocol(1);
    emitter.setPulseLength(190);
};

void EtekcityController::turnOn() {
    emitter.send((ETEKCITY_PREFIX + ETEKCITY_DEVICE_CODE[device-1] + ETEKCITY_COMMAND_ON).c_str());
}

void EtekcityController::turnOff() {
    emitter.send((ETEKCITY_PREFIX + ETEKCITY_DEVICE_CODE[device-1] + ETEKCITY_COMMAND_OFF).c_str());
}