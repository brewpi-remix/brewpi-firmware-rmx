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

#include "Devices.h"

/**
 * Actuator needs to be virtual so that instance pointers can be destroyed via the virtual destructor.
 */
#define ACTUATOR_VIRTUAL 1

#if ACTUATOR_VIRTUAL
#define ACTUATOR_METHOD virtual
#define ACTUATOR_METHOD_IMPL = 0
#define ACTUATOR_BASE_CLASS_DECL : public Actuator
#else
#define ACTUATOR_METHOD inline
#define ACTUATOR_METHOD_IMPL \
	{                        \
	}
#define ACTUATOR_BASE_CLASS_DECL
#endif

/*
 * An actuator simply turns something on or off.                        
 */

class Actuator
{
  public:
	ACTUATOR_METHOD void setActive(bool active) ACTUATOR_METHOD_IMPL;
	ACTUATOR_METHOD bool isActive() ACTUATOR_METHOD_IMPL;
#if ACTUATOR_VIRTUAL
	virtual ~Actuator()
	{
	}
#endif
};

/*
 * An actuator that simply remembers the set value. This is primary used for testing.
 */
class ValueActuator ACTUATOR_BASE_CLASS_DECL
{
  public:
	ValueActuator() : state(false) {}
	ValueActuator(bool initial) : state(initial) {}

	ACTUATOR_METHOD void setActive(bool active) { state = active; }
	ACTUATOR_METHOD bool isActive() { return state; }

  private:
	bool state;
};
