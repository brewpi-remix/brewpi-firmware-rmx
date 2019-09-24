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
#include "DisplayBase.h"
#include "DisplayLcdImpl.h"

class LcdDisplay DISPLAY_SUPERCLASS
{
  public:
	// Initializes the lcd display
	DISPLAY_METHOD void init(void);

	DISPLAY_METHOD void printAll()
	{
		printStationaryText();
		printState();
		printAllTemperatures();
		printMode();
	}

	// Print all temperatures on the LCD
	DISPLAY_METHOD void printAllTemperatures(void);

	// Print the stationary text on the lcd.
	DISPLAY_METHOD void printStationaryText(void);

	// Print mode on the right location on the first line, after Mode:
	DISPLAY_METHOD void printMode(void);

	DISPLAY_METHOD void setDisplayFlags(uint8_t newFlags);
	DISPLAY_METHOD uint8_t getDisplayFlags() { return flags; };

	// Print beer temperature at the right place on the display
	DISPLAY_METHOD void printBeerTemp(void);

	// Print beer temperature setting at the right place on the display
	DISPLAY_METHOD void printBeerSet(void);

	// Print fridge temperature at the right place on the display
	DISPLAY_METHOD void printFridgeTemp(void);

	// Print fridge temperature setting at the right place on the display
	DISPLAY_METHOD void printFridgeSet(void);

	// Print the current state on the last line of the LCD
	DISPLAY_METHOD void printState(void);

	DISPLAY_METHOD void getLine(uint8_t lineNumber, char *buffer) { lcd.getLine(lineNumber, buffer); }

	DISPLAY_METHOD void printAt_P(uint8_t x, uint8_t y, const char *text);

	DISPLAY_METHOD void setBufferOnly(bool bufferOnly)
	{
		lcd.setBufferOnly(bufferOnly);
	}

	DISPLAY_METHOD void resetBacklightTimer() { lcd.resetBacklightTimer(); }
	DISPLAY_METHOD void updateBacklight() { lcd.updateBacklight(); }

	// Print a temperature
	DISPLAY_METHOD void printTemperature(temperature temp);
	DISPLAY_METHOD void printTemperatureAt(uint8_t x, uint8_t y, temperature temp);

	// Print degree sign + C/F
	DISPLAY_METHOD void printDegreeUnit(uint8_t x, uint8_t y);

	DISPLAY_METHOD void printAt(uint8_t x, uint8_t y, char *text);

  private:
	DISPLAY_FIELD LcdDriver lcd;
	DISPLAY_FIELD uint8_t stateOnDisplay;
	DISPLAY_FIELD uint8_t flags;
};
