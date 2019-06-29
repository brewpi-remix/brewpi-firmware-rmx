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
#include "Ticks.h"
#include "Display.h"
#include "TempControl.h"
#include "PiLink.h"
#include "TempSensor.h"
#include "TempSensorMock.h"
#include "TempSensorExternal.h"
#include "Ticks.h"
#include "Sensor.h"
#include "SettingsManager.h"
#include "UI.h"
#include "RotaryEncoder.h"
#include <avr/wdt.h>

#if BREWPI_SIMULATE
#include "Simulator.h"
#endif

// Global class objects static and defined in class cpp and h files.
// Instantiate and configure the sensors, actuators and controllers
// we want to use.

void setup(void);
void loop(void);
#ifdef BREWPI_ROTARY_ENCODER
bool blankDisplay;
#endif

/*  
 *  Configure the counter and delay timer. The actual type of these will vary
 *  depending upon the environment. They are non-virtual to keep code size
 *  minimal, so typedefs and preprocessing are used to select the actual
 *  compile-time type used.
 */
TicksImpl ticks = TicksImpl(TICKS_IMPL_CONFIG);
DelayImpl wait = DelayImpl(DELAY_IMPL_CONFIG);

ValueActuator alarm;
UI ui;

void setup()
{
    ui.init();
    piLink.init();

    // logDebug("started");
    tempControl.init();
    settingsManager.loadSettings();

    uint32_t start = millis();
    uint32_t delay = ui.showStartupPage();
    while (millis() - start <= delay)
    {
        ui.ticks();
    }

#if BREWPI_SIMULATE
    simulator.step();
    // initialize the filters with the assigned initial temp value
    tempControl.beerSensor->init();
    tempControl.fridgeSensor->init();
#endif

    ui.showControllerPage();

    // If pushbutton is low on startup, disable backlight off timer
    pinMode(rotarySwitchPin, INPUT_PULLUP);
    blankDisplay = (digitalRead(rotarySwitchPin) == HIGH);

    // logDebug("init complete");
}

void brewpiLoop(void)
{
    static unsigned long lastUpdate = -1000;  // init at -1000 to update immediately
    uint8_t oldState;
    ui.ticks();

    // Reset display on timer to mitigate screen scramble
    #if BREWPI_LCD
        static unsigned long lastLcdUpdate = 0;  // Counter for LCD reset
        if (ticks.seconds() - lastLcdUpdate >= LCD_RESET_PERIOD)
        {
            lastLcdUpdate = ticks.seconds();

            display.init();
            display.printStationaryText();
            display.printState();

            
            #ifdef BREWPI_ROTARY_ENCODER
            rotaryEncoder.init();
            #endif
        }
    #endif

    if (!ui.inStartup() && (ticks.millis() - lastUpdate >= (1000)))
    { //update settings every second
        lastUpdate = ticks.millis();

        tempControl.updateTemperatures();
        tempControl.detectPeaks();
        tempControl.updatePID();
        oldState = tempControl.getState();
        tempControl.updateState();
        if (oldState != tempControl.getState())
        {
            piLink.printTemperatures(); // add a data point at every state transition
        }
        tempControl.updateOutputs();

        ui.update();
    }

    //listen for incoming serial connections while waiting to update
    piLink.receive();
}

void loop()
{
#if BREWPI_SIMULATE
    simulateLoop();
#else
    brewpiLoop();
#endif
}
