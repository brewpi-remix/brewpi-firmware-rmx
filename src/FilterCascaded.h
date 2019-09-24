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
#include "TemperatureFormats.h"
#include "FilterFixed.h"

// Use 3 filter sections. This gives excellent filtering, without adding too much delay.
// For 3 sections the stop band attenuation is 3x the single section attenuation in dB.
// The delay is also tripled.
#define NUM_SECTIONS 3

class CascadedFilter
{
  public:
	// CascadedFilter implements a filter that consists of multiple second order secions.
	FixedFilter sections[NUM_SECTIONS];

  public:
	CascadedFilter();
	~CascadedFilter() {}
	void init(temperature val);
	void setCoefficients(uint8_t bValue);
	temperature add(temperature val); // adds a value and returns the most recent filter output
	temperature_precise addDoublePrecision(temperature_precise val);
	temperature readInput(void); // returns the most recent filter input

	temperature readOutput(void)
	{
		return sections[NUM_SECTIONS - 1].readOutput(); // return output of last section
	}
	temperature_precise readOutputDoublePrecision(void);
	temperature_precise readPrevOutputDoublePrecision(void);

	temperature detectPosPeak(void)
	{
		return sections[NUM_SECTIONS - 1].detectPosPeak(); // detect peaks in last section
	}
	temperature detectNegPeak(void)
	{
		return sections[NUM_SECTIONS - 1].detectNegPeak(); // detect peaks in last section
	}
};
