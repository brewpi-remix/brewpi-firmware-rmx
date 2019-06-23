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
#include "Platform.h"
#include "PiLinkHandlers.h"
#include <avr/wdt.h>

// setup and loop are in brewpi_config so they can be reused across projects
extern void setup(void);
extern void loop(void);

void softwareReset( uint8_t prescaller) {
  // Start watchdog with the provided prescaller
  wdt_enable(prescaller);
  // Wait for the prescaller time to expire without sending the reset signal
  // by using the wdt_reset() method.
  while(1) {}
}

void handleReset()
{
	// Resetting using the watchdog timer (which is a full reset of all
	// registers) might not be compatible with old Arduino bootloaders.
	// Jumping to 0 is safer.
	// asm volatile("  jmp 0");
	
	// Restart in 60 milliseconds
	softwareReset(WDTO_60MS);
}

void flashFirmware()
{
	// A no-op. This is not used on this platform.
}

__attribute__((OS_main)) int main(void);

int main(void)
{
	init();

#if defined(USBCON)
	USBDevice.attach();
#endif

	setup();

	for (;;)
	{
		loop();
		if (serialEventRun)
			serialEventRun();
	}
	return 0;
}

// catch bad interrupts here, uncomment while only when debugging
ISR(BADISR_vect)
{
	while (1)
	;
}
