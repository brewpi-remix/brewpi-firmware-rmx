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
#include "TemperatureFormats.h"
#include "FilterFixed.h"
#include "FilterCascaded.h"
#include <stdlib.h>

CascadedFilter::CascadedFilter()
{
	for (uint8_t i = 0; i < NUM_SECTIONS; i++)
	{
		sections[i].setCoefficients(2); // default to a b value of 2
	}
}

void CascadedFilter::setCoefficients(uint8_t bValue)
{
	for (uint8_t i = 0; i < NUM_SECTIONS; i++)
	{
		sections[i].setCoefficients(bValue);
	}
}

temperature CascadedFilter::add(temperature val)
{
	temperature_precise valDoublePrecision = tempRegularToPrecise(val);
	valDoublePrecision = addDoublePrecision(valDoublePrecision);
	// return output, shifted back to single precision
	return tempPreciseToRegular(valDoublePrecision);
}

temperature_precise CascadedFilter::addDoublePrecision(temperature_precise val)
{
	temperature_precise input = val;
	// input is input for next section, which is the output of the previous section
	for (uint8_t i = 0; i < NUM_SECTIONS; i++)
	{
		input = sections[i].addDoublePrecision(input);
	}
	return input;
}

temperature CascadedFilter::readInput(void)
{
	return sections[0].readInput(); // return input of first section
}

temperature_precise CascadedFilter::readOutputDoublePrecision(void)
{
	return sections[NUM_SECTIONS - 1].readOutputDoublePrecision(); // return output of last section
}

temperature_precise CascadedFilter::readPrevOutputDoublePrecision(void)
{
	return sections[NUM_SECTIONS - 1].readPrevOutputDoublePrecision(); // return previous output of last section
}

void CascadedFilter::init(temperature val)
{
	for (uint8_t i = 0; i < NUM_SECTIONS; i++)
	{
		sections[i].init(val);
	}
}
