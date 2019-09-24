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
#include <stdint.h>

// Values returned by 'process'
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Anti-clockwise step.
#define DIR_CCW 0x20

class RotaryEncoder
{
  public:
	static void init(void);
	static void setRange(int16_t start, int16_t min, int16_t max);
	static void process(uint8_t currPinA, uint8_t currPinB);

	static bool changed(void); // returns one if the value changed since the last call of changed.
	static int16_t read(void);

	static int16_t readsteps(void)
	{
		return steps;
	}

	static bool pushed(void)
	{
		return pushFlag;
	}

	static void resetPushed(void)
	{
		pushFlag = false;
	}

	static void setPushed(void);

  private:
	static int16_t maximum;
	static int16_t minimum;
	static volatile int16_t steps;
	static volatile bool pushFlag;
};

extern RotaryEncoder rotaryEncoder;
