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

// Enable printing debug only log messages and debug only wrapped statements
#ifndef BREWPI_DEBUG
#define BREWPI_DEBUG 0
#endif

#if BREWPI_DEBUG > 0
#define DEBUG_ONLY(x) x
#else
#define DEBUG_ONLY(x)
#endif

// Set which debug messages are printed
#ifndef BREWPI_LOG_ERRORS
#define BREWPI_LOG_ERRORS 1
#endif

#ifndef BREWPI_LOG_WARNINGS
#define BREWPI_LOG_WARNINGS 1
#endif

#ifndef BREWPI_LOG_INFO
#define BREWPI_LOG_INFO 1
#endif

#ifndef BREWPI_LOG_DEBUG
#define BREWPI_LOG_DEBUG 0
#endif

/**
 * This flag virtualizes as much of the hardware as possible, so the code can
 * be run in the AvrStudio simulator, which only emulates the microcontroller,
 * not any attached peripherals.
 */
#ifndef BREWPI_EMUFLATE
#define BREWPI_EMULATE 0
#endif

#ifndef TEMP_SENSOR_CASCADED_FILTER
#define TEMP_SENSOR_CASCADED_FILTER 1
#endif

#ifndef TEMP_CONTROL_STATIC
#define TEMP_CONTROL_STATIC 1
#endif

/**
 * Enable the simulator. Real sensors/actuators are replaced with simulated
 * versions. In particular, the values reported by temp sensors are based on
 * a model of the fridge/beer.
 */
#ifndef BREWPI_SIMULATE
#define BREWPI_SIMULATE 0
#endif

#ifndef BREWPI_EEPROM_HELPER_COMMANDS
#define BREWPI_EEPROM_HELPER_COMMANDS BREWPI_DEBUG || BREWPI_SIMULATE
#endif

#ifndef OPTIMIZE_GLOBAL
#define OPTIMIZE_GLOBAL 1
#endif

#ifndef BUILD_NAME
#define BUILD_NAME "unknown"
#endif

/**
 * Enable the LCD menu.
 */
#ifndef BREWPI_MENU
#define BREWPI_MENU 1
#endif

#ifndef DISPLAY_TIME_HMS
#define DISPLAY_TIME_HMS 1
#endif
