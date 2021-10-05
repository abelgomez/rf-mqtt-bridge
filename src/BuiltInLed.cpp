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

#include "BuiltInLed.h"

void BuiltInLed::on() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void BuiltInLed::off() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void BuiltInLed::blink() {
    for (unsigned int i = 0; i < 10; i++) {
        on();
        delay(200);
        off();
        delay(100);
    }
}