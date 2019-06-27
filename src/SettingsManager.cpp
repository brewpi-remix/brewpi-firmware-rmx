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
#include "SettingsManager.h"
#include "TempControl.h"
#include "PiLink.h"
#include "TempSensorExternal.h"

void SettingsManager::loadSettings()
{
	// logDebug("loading settings");

	if (!eepromManager.applySettings())
	{
		tempControl.loadDefaultSettings();
		tempControl.loadDefaultConstants();

		deviceManager.setupUnconfiguredDevices();

		logWarning(WARNING_START_IN_SAFE_MODE);
	}

#if (BREWPI_SIMULATE)
	{
		// logDebug("Setting up simulator devices.");

		// temp sensors are special in the simulator - make sure they are set up even if not
		// configured in the eeprom
		DeviceConfig cfg;
		clear((uint8_t *)&cfg, sizeof(cfg));
		cfg.deviceHardware = DEVICE_HARDWARE_ONEWIRE_TEMP;
		cfg.chamber = 1;
		cfg.deviceFunction = DEVICE_CHAMBER_ROOM_TEMP;
		deviceManager.uninstallDevice(cfg);
		deviceManager.installDevice(cfg);

		cfg.deviceFunction = DEVICE_CHAMBER_TEMP;
		deviceManager.uninstallDevice(cfg);
		deviceManager.installDevice(cfg);

		cfg.beer = 1;
		cfg.deviceFunction = DEVICE_BEER_TEMP;
		deviceManager.uninstallDevice(cfg);
		deviceManager.installDevice(cfg);
	}
#endif
}

SettingsManager settingsManager;