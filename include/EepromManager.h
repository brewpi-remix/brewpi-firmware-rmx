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
#include "Platform.h"
#include "EepromAccess.h"

void fill(int8_t *p, uint8_t size);
void clear(uint8_t *p, uint8_t size);

class DeviceConfig;

// todo - the Eeprom manager should avoid too frequent saves to the eeprom since it supports 100,000 writes.
class EepromManager
{
  public:
	EepromManager();

	/**
	 * Write -1 to the entire eeprom, emulating the reset performed by avrdude.
	 */
	static void zapEeprom();

	/**
	 * Prepare the eeprom to accept device definitions. For RevA boards, the eeprom is populated with devices for
	 * beer/fridge temp sensor, and heating,cooling actuators and door switch.
	 */
	static void initializeEeprom();

	/**
	 * Determines if this eeprom has settings.
	 */
	static bool hasSettings();

	/**
	 * Applies the settings from the eeprom
	 */
	static bool applySettings();

	static void dumpEeprom(Print &stream, uint16_t offset);

	/**
	 * Save the chamber constants and beer settings to eeprom for the currently active chamber.
	 */
	static void storeTempConstantsAndSettings();

	/**
	 * Save just the beer temp settings.
	 */
	static void storeTempSettings();

	static bool fetchDevice(DeviceConfig &config, uint8_t deviceIndex);
	static bool storeDevice(const DeviceConfig &config, uint8_t deviceIndex);

	static uint8_t saveDefaultDevices();
};

class EepromStream
{
	eptr_t pv;

	void writeByte(uint8_t value)
	{
		eepromAccess.writeByte(pv++, value);
	}
	void writeBlock(void *source, uint16_t size)
	{
		eepromAccess.writeBlock(pv, source, size);
		pv += size;
	}
};

extern EepromManager eepromManager;
