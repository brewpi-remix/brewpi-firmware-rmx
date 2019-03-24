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
#include "TempSensor.h"
#include "PiLink.h"
#include "Ticks.h"

void TempSensor::init()
{
    logDebug("tempsensor::init - begin %d", failedReadCount);
    if (_sensor && _sensor->init() && failedReadCount > 60)
    {
        temperature temp = _sensor->read();
        if (temp != TEMP_SENSOR_DISCONNECTED)
        {
            logDebug("initializing filters with value %d", temp);
            fastFilter.init(temp);
            slowFilter.init(temp);
            slopeFilter.init(0);
            prevOutputForSlope = slowFilter.readOutputDoublePrecision();
            failedReadCount = 0;
        }
    }
}

void TempSensor::update()
{
    temperature temp;
    if (!_sensor || (temp = _sensor->read()) == TEMP_SENSOR_DISCONNECTED)
    {
        if (failedReadCount < 255)
        { // limit
            failedReadCount++;
        }
        return;
    }

    fastFilter.add(temp);
    slowFilter.add(temp);

    // update slope filter every 3 samples.
    // averaged differences will give the slope. Use the slow filter as input
    updateCounter--;
    // initialize first read for slope filter after (255-4) seconds. This prevents an influence for the startup inaccuracy.
    if (updateCounter == 4)
    {
        // only happens once after startup.
        prevOutputForSlope = slowFilter.readOutputDoublePrecision();
    }
    if (updateCounter <= 0)
    {
        temperature_precise slowFilterOutput = slowFilter.readOutputDoublePrecision();
        temperature_precise diff = slowFilterOutput - prevOutputForSlope;
        temperature diff_upper = diff >> 16;
        if (diff_upper > 27)
        { // limit to prevent overflow INT_MAX/1200 = 27.14
            diff = (27l << 16);
        }
        else if (diff_upper < -27)
        {
            diff = (-27l << 16);
        }
        slopeFilter.addDoublePrecision(1200 * diff); // Multiply by 1200 (1h/4s), shift to single precision
        prevOutputForSlope = slowFilterOutput;
        updateCounter = 3;
    }
}

temperature TempSensor::readFastFiltered(void)
{
    return fastFilter.readOutput(); //return most recent unfiltered value
}

temperature TempSensor::readSlope(void)
{
    // return slope per hour.
    temperature_precise doublePrecision = slopeFilter.readOutputDoublePrecision();
    return doublePrecision >> 16; // shift to single precision
}

temperature TempSensor::detectPosPeak(void)
{
    return slowFilter.detectPosPeak();
}

temperature TempSensor::detectNegPeak(void)
{
    return slowFilter.detectNegPeak();
}

void TempSensor::setFastFilterCoefficients(uint8_t b)
{
    fastFilter.setCoefficients(b);
}

void TempSensor::setSlowFilterCoefficients(uint8_t b)
{
    slowFilter.setCoefficients(b);
}

void TempSensor::setSlopeFilterCoefficients(uint8_t b)
{
    slopeFilter.setCoefficients(b);
}

BasicTempSensor &TempSensor::sensor()
{
    return *_sensor;
}
