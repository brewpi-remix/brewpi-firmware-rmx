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

// have to use two levels of macro expansion to convert a symbol to
// a string. see http://gcc.gnu.org/onlinedocs/cpp/Stringification.html
#define stringify(s) _stringifyDo(s)
#define _stringifyDo(s) #s

// Most pins are only conditionally defined here, allowing definitions
// to be provided in Config.h for local overrides
#define BREWPI_SHIELD_DIY 0
#define BREWPI_SHIELD_REVA 1
#define BREWPI_SHIELD_REVC 2
#define BREWPI_SHIELD_SPARK_REV_C 3     // Here for reference - this isn't actually supported in this firmware
#define BREWPI_SHIELD_SPARK_REV_C2 4    // Here for reference - this isn't actually supported in this firmware
#define BREWPI_SHIELD_I2C 5             // For hardware Uno shields with I2C on A4/A5
#define BREWPI_SHIELD_GLYCOL 6          // For hardware Uno shields with I2C on A4/A5

#define BREWPI_BOARD_LEONARDO 'l'
#define BREWPI_BOARD_STANDARD 's'
#define BREWPI_BOARD_MEGA 'm'
#define BREWPI_BOARD_SPARKCORE 'x'  // Here for reference - this isn't actually supported in this firmware
#define BREWPI_BOARD_PHOTON 'y'     // Here for reference - this isn't actually supported in this firmware
#define BREWPI_BOARD_UNKNOWN '?'

/*
 * Defines global config for the brewpi project. This file is included in
 * every file in the project to ensure conditional compilation directives
 * are recognized.
 * 
 * ConfigDefault.h contains the default settings, and produces a standard
 * Hex file.  To customize the build, users may add settings to Config.h,
 * or define symbols in the project.
 */

void watchdogSetup(void);

#ifdef ARDUINO
#include "Config.h"
#else
#include <Config.h> // use search path rather than current directory, so that config.h is found elsewhere
#endif
#include "ConfigDefault.h"

#ifdef ARDUINO
#include "AppConfig.h"
#else
#include <AppConfig.h>
#endif
#include "AppConfigDefault.h"

#ifndef BREWPI_H
#define BREWPI_H
#ifdef BREWPI_ROTARY_ENCODER
#include "Pins.h"
extern bool blankDisplay;
#endif
#endif /* BREWPI_H */