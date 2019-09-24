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
#include <stdint.h>

struct UI
{
    /**
     * Initialize the UI components.
     */
    static uint8_t init();

    /**
     * Display the startup page and retrieve how long the startup page should be displayed 
     * for in milliseconds before moving on to the main page.
     */
    static uint32_t showStartupPage();

    /**
     * Show the main controller page. 
     */
    static void showControllerPage();

    /**
     * Called as often as possible by the application. Allows the UI to perform
     * quick background tasks, such as reading the touch screen.
     */
    static void ticks();

    /**
     * Called from the main loop to update the current page and perform event
     * processing.
     */
    static void update();

    /**
     * Determines if this UI is in startup mode. The controller services piLink
     * requests and continues to update the UI, but the main control loop is not executed.
     * @return 
     */
    static bool inStartup();
};

extern UI ui;
