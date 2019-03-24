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

#include <avr/eeprom.h>
#include "EepromTypes.h"

class ArduinoEepromAccess
{
  public:
	static uint8_t readByte(eptr_t offset)
	{
		return eeprom_read_byte((uint8_t *)offset);
	}
	static void writeByte(eptr_t offset, uint8_t value)
	{
		eeprom_update_byte((uint8_t *)offset, value);
	}

	static void readBlock(void *target, eptr_t offset, uint16_t size)
	{
		eeprom_read_block(target, (uint8_t *)offset, size);
	}
	static void writeBlock(eptr_t target, const void *source, uint16_t size)
	{
		eeprom_update_block(source, (void *)target, size);
	}
};
