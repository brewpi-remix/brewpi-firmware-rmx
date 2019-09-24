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
#include "FilterCascaded.h"
#include "TempSensorBasic.h"
#include <stdlib.h>

#define TEMP_SENSOR_DISCONNECTED INVALID_TEMP

#ifndef TEMP_SENSOR_CASCADED_FILTER
#define TEMP_SENSOR_CASCADED_FILTER 1
#endif

#if TEMP_SENSOR_CASCADED_FILTER
typedef CascadedFilter TempSensorFilter;
#else
typedef FixedFilter TempSensorFilter;
#endif

enum TempSensorType
{
	TEMP_SENSOR_TYPE_FRIDGE = 1,
	TEMP_SENSOR_TYPE_BEER
};

class TempSensor
{
  public:
	TempSensor(TempSensorType sensorType, BasicTempSensor *sensor = NULL)
	{
		updateCounter = 255; // first update for slope filter after (255-4s)
		setSensor(sensor);
	}

	void setSensor(BasicTempSensor *sensor)
	{
		_sensor = sensor;
		failedReadCount = 255;
	}

	bool hasSlowFilter() { return true; }
	bool hasFastFilter() { return true; }
	bool hasSlopeFilter() { return true; }

	void init();

	bool isConnected() { return _sensor != NULL && _sensor->isConnected(); }

	void update();

	temperature readFastFiltered(void);

	temperature readSlowFiltered(void)
	{
		return slowFilter.readOutput(); //return most recent unfiltered value
	}

	temperature readSlope(void);

	temperature detectPosPeak(void);

	temperature detectNegPeak(void);

	void setFastFilterCoefficients(uint8_t b);

	void setSlowFilterCoefficients(uint8_t b);

	void setSlopeFilterCoefficients(uint8_t b);

	BasicTempSensor &sensor();

  private:
	BasicTempSensor *_sensor;
	TempSensorFilter fastFilter;
	TempSensorFilter slowFilter;
	TempSensorFilter slopeFilter;
	unsigned char updateCounter;
	temperature_precise prevOutputForSlope;

	// An indication of how stale the data is in the filters. Each time a
	// read fails, this value is incremented. It's used to reset the filters
	// after a large enough disconnect delay, and on the first init.
	uint8_t failedReadCount;

	friend class ChamberManager;
	friend class Chamber;
	friend class DeviceManager;
};
