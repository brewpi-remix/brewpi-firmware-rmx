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
#include "BrewpiStrings.h"
#include <stdint.h>
#include <Print.h>
#include "Ticks.h"

class NullLcdDriver : public Print
{
  public:
	// Constants are set in initializer list of constructor
	NullLcdDriver(){};
	~NullLcdDriver(){};

	void init();

	void begin(uint8_t cols, uint8_t rows);

	void clear();
	void home();

	void noDisplay();
	void display();
	void noBlink();
	void blink();
	void noCursor();
	void cursor();
	void scrollDisplayLeft();
	void scrollDisplayRight();
	void leftToRight();
	void rightToLeft();
	void autoscroll();
	void noAutoscroll();

	void createChar(uint8_t, uint8_t[]);
	void setCursor(uint8_t, uint8_t);

	virtual size_t write(uint8_t);

	void print_P(const char *str)
	{						// print a string stored in PROGMEM
		char buf[21];		// create buffer in RAM
		strcpy_P(buf, str); // copy string to RAM
		print(buf);			// print from RAM
	}

	// copy a line from the shadow copy to a string buffer and correct the degree sign
	void getLine(uint8_t lineNumber, char *buffer);

	void readContent(void); // read the content from the display to the shadow copy buffer

	void command(uint8_t);
	char readChar(void);

	void setBufferOnly(bool bufferOnly) { _bufferOnly = bufferOnly; }

	void resetBacklightTimer(void);

	void updateBacklight(void);

	uint8_t getCurrPos(void)
	{
		return _currpos;
	}
	uint8_t getCurrLine(void)
	{
		return _currline;
	}

	// Write spaces from current position to line end.
	void printSpacesToRestOfLine(void);

	using Print::write;

  private:
	uint8_t _currline;
	uint8_t _currpos;
	uint8_t _numlines;

	bool _bufferOnly;

	char content[4][21]; // always keep a copy of the display content in this variable
};
