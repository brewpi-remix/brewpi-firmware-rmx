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
#include <stdlib.h>

temperature FixedFilter::add(temperature val)
{
	temperature_precise returnVal = addDoublePrecision(tempRegularToPrecise(val));
	return tempPreciseToRegular(returnVal);
}

temperature_precise FixedFilter::addDoublePrecision(temperature_precise val)
{
	xv[2] = xv[1];
	xv[1] = xv[0];
	xv[0] = val;

	yv[2] = yv[1];
	yv[1] = yv[0];

	/* Implementation that prevents overflow as much as possible by order of operations: */
	yv[0] = ((yv[1] - yv[2]) + yv[1])						  // expected value + 1*
			- (yv[1] >> b) + (yv[2] >> b) +					  // expected value +0*
			+(xv[0] >> a) + (xv[1] >> (a - 1)) + (xv[2] >> a) // expected value +(1>>(a-2))
			- (yv[2] >> (a - 2));							  // expected value -(1>>(a-2))

	return yv[0];
}

void FixedFilter::init(temperature val)
{
	xv[0] = val;
	xv[0] = tempRegularToPrecise(xv[0]); // 16 extra bits are used in the filter for the fraction part

	xv[1] = xv[0];
	xv[2] = xv[0];

	yv[0] = xv[0];
	yv[1] = xv[0];
	yv[2] = xv[0];
}

temperature FixedFilter::detectPosPeak(void)
{
	if (yv[0] < yv[1] && yv[1] >= yv[2])
	{
		return tempPreciseToRegular(yv[1]);
	}
	else
	{
		return INVALID_TEMP;
	}
}

temperature FixedFilter::detectNegPeak(void)
{
	if (yv[0] > yv[1] && yv[1] <= yv[2])
	{
		return tempPreciseToRegular(yv[1]);
	}
	else
	{
		return INVALID_TEMP;
	}
}
