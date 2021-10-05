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

#include "NovyController.h"

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
static const String NOVY_DEVICE_CODE[] = {
    "0101",
    "1001",
    "0001",
    "1110",
    "0110",
    "1010",
    "0010",
    "1100",
    "0100",
    "1000",
};

// Prefix (next 4 bits, not sure what it means, if it is always fixed, or if it is part of the command code)
static const String NOVY_PREFIX = "0101";

// Comamands to be sent (code + prefix + command)
static const String NOVY_COMMAND_LIGHT = "0111010001";
static const String NOVY_COMMAND_POWER = "0111010011";
static const String NOVY_COMMAND_PLUS =  "0101";
static const String NOVY_COMMAND_MINUS = "0110";
static const String NOVY_COMMAND_NOVY =  "0100";


NovyController::NovyController(uint8_t pin, ushort device) : pin(pin), device(device) {
    emitter.enableTransmit(pin);
    emitter.setProtocol(12);
    emitter.setPulseLength(350);
};

void NovyController::pressLight() {
    emitter.send((NOVY_DEVICE_CODE[device-1] + NOVY_PREFIX + NOVY_COMMAND_LIGHT).c_str());
}

void NovyController::pressMinus() {
    emitter.send((NOVY_DEVICE_CODE[device-1] + NOVY_PREFIX + NOVY_COMMAND_MINUS).c_str());
}

void NovyController::pressPlus() {
    emitter.send((NOVY_DEVICE_CODE[device-1] + NOVY_PREFIX + NOVY_COMMAND_PLUS).c_str());
}

void NovyController::pressPower() {
    emitter.send((NOVY_DEVICE_CODE[device-1] + NOVY_PREFIX + NOVY_COMMAND_POWER).c_str());
}

void NovyController::pressNovy() {
    emitter.send((NOVY_DEVICE_CODE[device-1] + NOVY_PREFIX + NOVY_COMMAND_NOVY).c_str());
}

void NovyController::lightOn() {
    lightOff();
    delay(400);
    pressLight();
}

void NovyController::lightOff() {
    for (uint i = 0; i < 5; i++) {
        pressLight();
        delay(100);
    }
    delay(300);
    pressLight();
}

void NovyController::fanOff() {
    for (uint i = 0; i < 4; i++) {
        emitter.send((NOVY_DEVICE_CODE[device-1] + NOVY_PREFIX + NOVY_COMMAND_MINUS).c_str());
        delay(200);
    }
}

void NovyController::fan(ushort level) {
    fanOff();   
    for (uint i = 0; i < level; i++) {
        emitter.send((NOVY_DEVICE_CODE[device-1] + NOVY_PREFIX + NOVY_COMMAND_PLUS).c_str());
        delay(200);
    }
}
