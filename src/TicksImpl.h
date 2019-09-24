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

#ifndef TICKSIMPL_H_
#define TICKSIMPL_H_

typedef uint16_t tcduration_t;
typedef uint32_t ticks_millis_t;
typedef uint32_t ticks_micros_t;
typedef uint16_t ticks_seconds_t;
typedef uint8_t ticks_seconds_tiny_t;

#include "TicksWiring.h"

// Determine the type of Ticks needed
// TICKS_IMPL_CONFIG is the code string passed to the constructor of the Ticks implementation

#if BREWPI_SIMULATE
/** For simulation, by the simulator - each step in the simulator advances the time by one second. */
typedef ExternalTicks TicksImpl;
#define TICKS_IMPL_CONFIG // no configuration of ExternalTicks necessary

#elif BREWPI_EMULATE
/** When debugging in AVR studio (and running normal brewpi - not the simulator), use a simple MockTicks that increments 100
	millis each time it's called. */
typedef MockTicks TicksImpl;
#define TICKS_IMPL_CONFIG 100

#else // use regular hardware timer/delay
typedef HardwareTicks TicksImpl;
#define TICKS_IMPL_CONFIG
#endif // BREWPI_EMULATE

extern TicksImpl ticks;

// Determine the type of delay required.
// For emulation, don't delay, since time in the emulator is not real time, so the delay is meaningless.
// For regular code, use the arduino delay function.

#if BREWPI_EMULATE || !defined(ARDUINO)
typedef NoOpDelay DelayImpl; // for emulation (avr debugger), don't bother delaying, it takes ages.
#define DELAY_IMPL_CONFIG
#else
typedef HardwareDelay DelayImpl;
#define DELAY_IMPL_CONFIG
#endif

extern DelayImpl wait;

#endif /* TICKSIMPL_H_ */
