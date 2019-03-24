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
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "NullLcdDriver.h"

void NullLcdDriver::init()
{
}

void NullLcdDriver::begin(uint8_t cols, uint8_t lines)
{
	_numlines = lines;
	_currline = 0;
	_currpos = 0;

	clear();
	home();
	display();
}

/********** high level commands, for the user! */
void NullLcdDriver::clear()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		for (uint8_t j = 0; j < 20; j++)
		{
			content[i][j] = ' '; // initialize on all spaces
		}
		content[i][20] = '\0'; // NULL terminate string
	}
}

void NullLcdDriver::home()
{
	_currline = 0;
	_currpos = 0;
}

void NullLcdDriver::setCursor(uint8_t col, uint8_t row)
{
	if (row >= _numlines)
	{
		row = 0; //write to first line if out off bounds
	}
	_currline = row;
	_currpos = col;
}

// Turn the display on/off (quickly)
void NullLcdDriver::noDisplay()
{
}
void NullLcdDriver::display()
{
}

// Turns the underline cursor on/off
void NullLcdDriver::noCursor()
{
}
void NullLcdDriver::cursor()
{
}

// Turn on and off the blinking cursor
void NullLcdDriver::noBlink()
{
}
void NullLcdDriver::blink()
{
}

// These commands scroll the display without changing the RAM
void NullLcdDriver::scrollDisplayLeft(void)
{
}
void NullLcdDriver::scrollDisplayRight(void)
{
}

// This is for text that flows Left to Right
void NullLcdDriver::leftToRight(void)
{
}

// This is for text that flows Right to Left
void NullLcdDriver::rightToLeft(void)
{
}

// This will 'right justify' text from the cursor
void NullLcdDriver::autoscroll(void)
{
}

// This will 'left justify' text from the cursor
void NullLcdDriver::noAutoscroll(void)
{
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void NullLcdDriver::createChar(uint8_t location, uint8_t charmap[])
{
	location &= 0x7; // we only have 8 locations 0-7
	for (int i = 0; i < 8; i++)
	{
		write(charmap[i]);
	}
}

// This resets the backlight timer and updates the SPI output
void NullLcdDriver::resetBacklightTimer(void)
{
}

void NullLcdDriver::updateBacklight(void)
{
}

// Puts the content of one LCD line into the provided buffer.
void NullLcdDriver::getLine(uint8_t lineNumber, char *buffer)
{
	const char *src = content[lineNumber];
	for (uint8_t i = 0; i < 20; i++)
	{
		uint8_t c = src[i];
		buffer[i] = (c == 0b11011111) ? 0xB0 : c;
	}
	buffer[20] = '\0'; // NULL terminate string
}

/*********** mid level commands, for sending data/cmds */

inline void NullLcdDriver::command(uint8_t value)
{
}

size_t NullLcdDriver::write(uint8_t value)
{
	content[_currline][_currpos] = value;
	_currpos++;
	return 1;
}

void NullLcdDriver::printSpacesToRestOfLine(void)
{
	while (_currpos < 20)
	{
		print(' ');
	}
}
