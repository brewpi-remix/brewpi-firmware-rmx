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

#include "Brewpi.h"
#include "Platform.h"
#include "BrewpiStrings.h"

// some useful strings
const char STR_FMT_S_RAM[] PROGMEM = "%s";	 // RAM string
const char STR_FMT_S_PROGMEM[] PROGMEM = "%S"; // PROGMEM string
const char STR_FMT_D[] PROGMEM = "%d";
const char STR_FMT_U[] PROGMEM = "%u";
const char STR_6SPACES[] PROGMEM = "      ";

#if !indexOf_inline
int8_t indexOf(const char *s, char c)
{
	char c2;
	int8_t idx = -1;
	while ((c2 = s[++idx]))
	{
		if (c == c2)
			return idx;
	}
	return -1;
}
#endif
