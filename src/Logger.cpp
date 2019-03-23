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
#include "Platform.h"
#include "BrewpiStrings.h"
#include "Logger.h"
#include "PiLink.h"
#include "JsonKeys.h"

static const char PROGMEM LOG_STRING_FORMAT[] = "\"%s\"";

void Logger::logMessageVaArg(char type, LOG_ID_TYPE errorID, const char *varTypes, ...)
{
	va_list args;
	piLink.printResponse('D');
	piLink.sendJsonPair(JSONKEY_logType, type);
	piLink.sendJsonPair(JSONKEY_logID, errorID);
	piLink.print_P(PSTR(",\"V\":["));
	va_start(args, varTypes);
	uint8_t index = 0;
	char buf[9];
	while (varTypes[index])
	{
		switch (varTypes[index])
		{
		case 'd': // integer, signed or unsigned
			piLink.print_P(STR_FMT_D, va_arg(args, int));
			break;
		case 's': // string
			piLink.print_P(LOG_STRING_FORMAT, va_arg(args, char *));
			break;
		case 't': // temperature in fixed_7_9 format
			piLink.print_P(LOG_STRING_FORMAT, tempToString(buf, va_arg(args, int), 1, 12));
			break;
		case 'f': // fixed point value
			piLink.print_P(LOG_STRING_FORMAT, fixedPointToString(buf, (temperature)va_arg(args, int), 3, 12));
			break;
		}
		if (varTypes[++index])
		{
			piLink.print(',');
		}
	}
	va_end(args);
	piLink.print(']');
	piLink.sendJsonClose();
}

Logger logger;