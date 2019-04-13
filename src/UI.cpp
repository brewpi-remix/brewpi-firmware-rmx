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
#include "UI.h"
#include "RotaryEncoder.h"
#include "Display.h"
#include "Buzzer.h"
#include "Menu.h"
#include "Actuator.h"

DisplayType realDisplay;
DisplayType DISPLAY_REF display = realDisplay;

uint8_t UI::init()
{
#if BREWPI_BUZZER
	buzzer.init();
	buzzer.beep(2, 500);
#endif
	display.init();
	rotaryEncoder.init();
	return 0;
}

uint32_t UI::showStartupPage()
{
	return 0;
}

/**
 * Show the main controller page. 
 */
void UI::showControllerPage()
{
	display.printStationaryText();
	display.printState();
}

extern ValueActuator alarm;
void UI::ticks()
{
#if BREWPI_BUZZER
	buzzer.setActive(alarm.isActive() && !buzzer.isActive());
#endif

#if BREWPI_MENU
	if (rotaryEncoder.pushed())
	{
		rotaryEncoder.resetPushed();
		menu.pickSettingToChange();
	}
#endif
}

void UI::update()
{
	// update the lcd for the chamber being displayed
	display.printState();
	display.printAllTemperatures();
	display.printMode();
	display.updateBacklight();
}

bool UI::inStartup() { return false; }
