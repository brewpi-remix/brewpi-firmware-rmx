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

/**
 * Do not change this file directly - rather edit Config.h
 */

#ifndef BREWPI_STATIC_CONFIG
#define BREWPI_STATIC_CONFIG BREWPI_SHIELD_REVC
#endif

#if BREWPI_STATIC_CONFIG == BREWPI_SHIELD_I2C
#define BREWPI_I2C 1
#endif

/*
 * LCD Display using a shift register:
 * For DIY-shields prior to the revA shield, this should be set to 0. With
 * IIC support, we want BREWPI_SHIFT_LCD set to 0 if BREWPI_I2C is set.
 */

#ifndef BREWPI_SHIFT_LCD
#if BREWPI_STATIC_CONFIG != BREWPI_SHIELD_DIY && !defined(BREWPI_I2C)
#define BREWPI_SHIFT_LCD 1
#else
#define BREWPI_SHIFT_LCD 0
#endif
#endif

#ifndef FAST_DIGITAL_PIN
#define FAST_DIGITAL_PIN 0
#endif

/**
 * Enable DS2413 Actuators. 
 */
#ifndef BREWPI_DS2413
#define BREWPI_DS2413 0
#endif

/**
 * Enable the LCD display. Without this, a NullDisplay is used
 */
#ifndef BREWPI_LCD
#define BREWPI_LCD 1
#endif

#ifndef BREWPI_BUZZER
#if BREWPI_STATIC_CONFIG == BREWPI_SHIELD_DIY
#define BREWPI_BUZZER 0
#else
#define BREWPI_BUZZER 1
#endif
#endif

#ifndef BREWPI_ROTARY_ENCODER
#if BREWPI_STATIC_CONFIG == BREWPI_SHIELD_DIY
#define BREWPI_ROTARY_ENCODER 0
#else
#define BREWPI_ROTARY_ENCODER 1
#endif
#endif

// BREWPI_SENSOR_PINS  - Enabled if not using onewire sensors
#ifndef BREWPI_SENSOR_PINS
#define BREWPI_SENSOR_PINS 0
#endif

// BREWPI_ACTUATOR_PINS - Enabled if not using onewire actuators
#ifndef BREWPI_ACTUATOR_PINS
#define BREWPI_ACTUATOR_PINS 1
#endif

#ifndef BREWPI_BOARD

#if !ARDUINO
#define BREWPI_BOARD BREWPI_BOARD_UNKNOWN
#elif defined(__AVR_ATmega32U4__)
#define BREWPI_BOARD BREWPI_BOARD_LEONARDO
#elif defined(__AVR_ATmega328P__)
#define BREWPI_BOARD BREWPI_BOARD_STANDARD
#elif defined(__AVR_ATmega2560__)
#define BREWPI_BOARD BREWPI_BOARD_MEGA
#else
#error Unknown processor type!
#define BREWPI_BOARD BREWPI_BOARD_UNKNOWN
#endif

#endif // ifndef BREWPI_BOARD

/*
 * Disable onewire crc table - it takes up 256 bytes of progmem.
 */
#ifndef ONEWIRE_CRC8_TABLE
#define ONEWIRE_CRC8_TABLE 0
#endif

#ifndef ONEWIRE_PARASITE_SUPPORT
#define ONEWIRE_PARASITE_SUPPORT 0
#endif

#ifndef DS2413_SUPPORT_SENSE
#define DS2413_SUPPORT_SENSE 0
#endif
