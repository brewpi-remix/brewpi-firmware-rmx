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

#include "DeviceManager.h"
#include "Pins.h"

#if !BREWPI_SIMULATE
#if BREWPI_STATIC_CONFIG <= BREWPI_SHIELD_REV_A
OneWire beerSensorBus(beerSensorPin);
OneWire fridgeSensorBus(fridgeSensorPin);
#elif BREWPI_STATIC_CONFIG >= BREWPI_SHIELD_REV_C
OneWire primaryOneWireBus(oneWirePin);
#endif
#endif

OneWire *DeviceManager::oneWireBus(uint8_t pin)
{
#if !BREWPI_SIMULATE
#if BREWPI_STATIC_CONFIG <= BREWPI_SHIELD_REV_A
    if (pin == beerSensorPin)
        return &beerSensorBus;
    if (pin == fridgeSensorPin)
        return &fridgeSensorBus;
#elif BREWPI_STATIC_CONFIG >= BREWPI_SHIELD_REV_C
    if (pin == oneWirePin)
        return &primaryOneWireBus;
#endif
#endif
    return NULL;
}

int8_t DeviceManager::enumerateActuatorPins(uint8_t offset)
{
#if BREWPI_STATIC_CONFIG <= BREWPI_SHIELD_REV_A
    switch (offset)
    {
    case 0:
        return heatingPin;
    case 1:
        return coolingPin;
    default:
        return -1;
    }
#elif BREWPI_STATIC_CONFIG >= BREWPI_SHIELD_REV_C
    switch (offset)
    {
    case 0:
        return actuatorPin1;
    case 1:
        return actuatorPin2;
    case 2:
        return actuatorPin3;
    case 3:
        return actuatorPin4;
    default:
        return -1;
    }
#endif
    return -1;
}

int8_t DeviceManager::enumerateSensorPins(uint8_t offset)
{
    if (offset == 0)
        return doorPin;
    return -1;
}

/* 
 * Enumerates the 1-wire pins.
 */
int8_t DeviceManager::enumOneWirePins(uint8_t offset)
{
#if BREWPI_STATIC_CONFIG <= BREWPI_SHIELD_REV_A
    if (offset == 0)
        return beerSensorPin;
    if (offset == 1)
        return fridgeSensorPin;
#endif
#if BREWPI_STATIC_CONFIG >= BREWPI_SHIELD_REV_C
    if (offset == 0)
        return oneWirePin;
#endif
    return -1;
}
