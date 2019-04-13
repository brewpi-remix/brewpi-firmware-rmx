/* Copyright (C) 2019 Lee C. Bussy (@LBussy)

This file is part of LBussy's BrewPi Firmware Remix (BrewPi-Firmware-RMX).

BrewPi Firmware RMX is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

BrewPi Firmware RMX is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with BrewPi Firmware RMX. If not, see <https://www.gnu.org/licenses/>.

These scripts were originally a part of firmware, a part of
the BrewPi project. Legacy support (for the very popular Arduino
controller) seems to have been discontinued in favor of new hardware.

All credit for the original firmware goes to @elcojacobs,
@m-mcgowan, @elnicoCZ, @ntfreak, @Gargy007 and I'm sure many more
contributors around the world. My apologies if I have missed anyone;
those were the names listed as contributors on the Legacy branch.

See: 'original-license.md' for notes about the original project's
license and credits. */

#pragma once

#include "Brewpi.h"

#if BREWPI_STATIC_CONFIG == BREWPI_SHIELD_REVA

#ifndef beerSensorPin
#define beerSensorPin A5 // OneWire 1
#endif

#ifndef fridgeSensorPin
#define fridgeSensorPin A4 // OneWire 2
#endif

#ifndef coolingPin
#define coolingPin 6
#endif

#ifndef heatingPin
#define heatingPin 5
#endif

#ifndef doorPin
#define doorPin 4
#endif

#ifndef alarmPin
#define alarmPin 3
#endif

#ifndef lcdLatchPin
#define lcdLatchPin 10
#endif

// If you change the interrupt pins, you will also have to review the interrupt vectors of the rotary encoder
#define rotarySwitchPin 7 // INT6 on leo or PCINT23 on uno
#define rotaryAPin 8      // PCINT4 on leo or PCINT0 on uno
#define rotaryBPin 9      // PCINT5 on leo or PCINT1 on uno

#define BREWPI_INVERT_ACTUATORS 1

#elif BREWPI_STATIC_CONFIG == BREWPI_SHIELD_REVC

#ifndef oneWirePin
#define oneWirePin A4
#endif

#ifndef actuatorPin1
#define actuatorPin1 2
#endif

#ifndef actuatorPin2
#define actuatorPin2 5
#endif

#ifndef actuatorPin3
#define actuatorPin3 6
#endif

#ifndef actuatorPin4
#define actuatorPin4 A5
#endif

#ifndef doorPin
#define doorPin 4
#endif

#ifndef alarmPin
#define alarmPin 3
#endif

#ifndef lcdLatchPin
#define lcdLatchPin 10
#endif

// If you change the interrupt pins, you will also have to review the interrupt vectors of the rotary encoder
#define rotarySwitchPin 7 // INT6 on leo or PCINT23 on uno
#define rotaryAPin 8      // PCINT4 on leo or PCINT0 on uno
#define rotaryBPin 9      // PCINT5 on leo or PCINT1 on uno

#define BREWPI_INVERT_ACTUATORS 1

#elif BREWPI_STATIC_CONFIG == BREWPI_SHIELD_DIY

// pins
#define beerSensorPin 10
#define fridgeSensorPin 11

// Pay attention when changing the pins for the rotary encoder.
// They should be connected to external interrupt INT0, INT1 and INT3

#define rotaryAPin 2      // INT1
#define rotaryBPin 1      // INT3
#define rotarySwitchPin 0 // INT2

#define coolingPin 12
#define heatingPin 13
#define doorPin A5

#define DISP_RS 9
#define DISP_RW 8
#define DISP_EN 7
#define DISP_D4 6
#define DISP_D5 5
#define DISP_D6 4
#define DISP_D7 3

#define BREWPI_INVERT_ACTUATORS 0

#elif BREWPI_STATIC_CONFIG == BREWPI_SHIELD_I2C

#ifndef oneWirePin
#define oneWirePin A0  // A4 in Rev C
#endif

#ifndef actuatorPin1
#define actuatorPin1 2  // NC
#endif

#ifndef actuatorPin2
#define actuatorPin2 5
#endif

#ifndef actuatorPin3
#define actuatorPin3 6
#endif

#ifndef actuatorPin4
#define actuatorPin4 A1  // TODO:  Check this (A5 in Rev C)
#endif

#ifndef doorPin
#define doorPin 4
#endif

#ifndef alarmPin
#define alarmPin 3
#endif

#ifndef lcdLatchPin
#define lcdLatchPin 10
#endif

// If you change the interrupt pins, you will also have to review the
// interrupt vectors of the rotary encoder
#define rotarySwitchPin 7 // INT6 on leo or PCINT23 on uno
#define rotaryAPin 8      // PCINT4 on leo or PCINT0 on uno
#define rotaryBPin 9      // PCINT5 on leo or PCINT1 on uno

// IIC pin definitions are not used, they are here for documentation
#define IIC_SDA  A4  
#define IIC_SCL  A5

#define BREWPI_INVERT_ACTUATORS 1

#endif

// You can use the internal pull-up resistors instead of external ones for
// the doorPin and the rotary encoder pins
#ifndef USE_INTERNAL_PULL_UP_RESISTORS
#define USE_INTERNAL_PULL_UP_RESISTORS 1
#endif
