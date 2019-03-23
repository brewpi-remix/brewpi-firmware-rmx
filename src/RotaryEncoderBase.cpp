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
#include "RotaryEncoder.h"

#include <limits.h>
#include "Ticks.h"
#include "Display.h"
#include "Brewpi.h"
#include "TempControl.h"

RotaryEncoder rotaryEncoder;

int16_t RotaryEncoder::maximum;
int16_t RotaryEncoder::minimum;
volatile int16_t RotaryEncoder::steps;
volatile bool RotaryEncoder::pushFlag;

// Implementation based on work of Ben Buxton:

/* Rotary encoder handler for arduino. v1.1
 *
 * Copyright 2011 Ben Buxton. Licenced under the GNU GPL Version 3.
 * Contact: bb@cactii.net
 *
 * A typical mechanical rotary encoder emits a two bit gray code
 * on 3 output pins. Every step in the output (often accompanied
 * by a physical 'click') generates a specific sequence of output
 * codes on the pins.
 *
 * There are 3 pins used for the rotary encoding - one common and
 * two 'bit' pins.
 *
 * The following is the typical sequence of code on the output when
 * moving from one step to the next:
 *
 *   Position   Bit1   Bit2
 *   ----------------------
 *     Step1     0      0
 *      1/4      1      0
 *      1/2      1      1
 *      3/4      0      1
 *     Step2     0      0
 *
 * From this table, we can see that when moving from one 'click' to
 * the next, there are 4 changes in the output code.
 *
 * - From an initial 0 - 0, Bit1 goes high, Bit0 stays low.
 * - Then both bits are high, halfway through the step.
 * - Then Bit1 goes low, but Bit2 stays high.
 * - Finally at the end of the step, both bits return to 0.
 *
 * Detecting the direction is easy - the table simply goes in the other
 * direction (read up instead of down).
 *
 * To decode this, we use a simple state machine. Every time the output
 * code changes, it follows state, until finally a full steps worth of
 * code is received (in the correct order). At the final 0-0, it returns
 * a value indicating a step in one direction or the other.
 *
 * It's also possible to use 'half-step' mode. This just emits an event
 * at both the 0-0 and 1-1 positions. This might be useful for some
 * encoders where you want to detect all positions.
 *
 * If an invalid state happens (for example we go from '0-1' straight
 * to '1-0'), the state machine resets to the start until 0-0 and the
 * next valid codes occur.
 *
 * The biggest advantage of using a state machine over other algorithms
 * is that this has inherent debounce built in. Other algorithms emit spurious
 * output with switch bounce, but this one will simply flip between
 * sub-states until the bounce settles, then continue along the state
 * machine.
 * A side effect of debounce is that fast rotations can cause steps to
 * be skipped. By not requiring debounce, fast rotations can be accurately
 * measured.
 * Another advantage is the ability to properly handle bad state, such
 * as due to EMI, etc.
 * It is also a lot simpler than others - a static state table and less
 * than 10 lines of logic.
 */

/*
 * The below state table has, for each state (row), the new state
 * to set based on the next encoder output. From left to right in,
 * the table, the encoder outputs are 00, 01, 10, 11, and the value
 * in that position is the new state to set.
 */

#define R_START 0x0
// #define HALF_STEP

// Use the half-step state table (emits a code at 00 and 11)
#define HS_R_CCW_BEGIN 0x1
#define HS_R_CW_BEGIN 0x2
#define HS_R_START_M 0x3
#define HS_R_CW_BEGIN_M 0x4
#define HS_R_CCW_BEGIN_M 0x5
const uint8_t PROGMEM hs_ttable[7][4] = {
    // R_START (00)
    {HS_R_START_M, HS_R_CW_BEGIN, HS_R_CCW_BEGIN, R_START},
    // HS_R_CCW_BEGIN
    {HS_R_START_M | DIR_CCW, R_START, HS_R_CCW_BEGIN, R_START},
    // HS_R_CW_BEGIN
    {HS_R_START_M | DIR_CW, HS_R_CW_BEGIN, R_START, R_START},
    // HS_R_START_M (11)
    {HS_R_START_M, HS_R_CCW_BEGIN_M, HS_R_CW_BEGIN_M, R_START},
    // HS_R_CW_BEGIN_M
    {HS_R_START_M, HS_R_START_M, HS_R_CW_BEGIN_M, R_START | DIR_CW},
    // HS_R_CCW_BEGIN_M
    {HS_R_START_M, HS_R_CCW_BEGIN_M, HS_R_START_M, R_START | DIR_CCW},
    {R_START, R_START, R_START, R_START}};

// Use the full-step state table (emits a code at 00 only)
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const uint8_t PROGMEM ttable[7][4] = {
    // R_START
    {R_START, R_CW_BEGIN, R_CCW_BEGIN, R_START},
    // R_CW_FINAL
    {R_CW_NEXT, R_START, R_CW_FINAL, R_START | DIR_CW},
    // R_CW_BEGIN
    {R_CW_NEXT, R_CW_BEGIN, R_START, R_START},
    // R_CW_NEXT
    {R_CW_NEXT, R_CW_BEGIN, R_CW_FINAL, R_START},
    // R_CCW_BEGIN
    {R_CCW_NEXT, R_START, R_CCW_BEGIN, R_START},
    // R_CCW_FINAL
    {R_CCW_NEXT, R_CCW_FINAL, R_START, R_START | DIR_CCW},
    // R_CCW_NEXT
    {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};

void RotaryEncoder::process(uint8_t currPinA, uint8_t currPinB)
{
    static uint8_t state = R_START;

    unsigned char pinstate = (currPinB << 1) | currPinA;

    // Determine new state from the pins and state table.
    if (tempControl.cc.rotaryHalfSteps)
    {
        state = pgm_read_byte(&(hs_ttable[state & 0xf][pinstate]));
    }
    else
    {
        state = pgm_read_byte(&(ttable[state & 0xf][pinstate]));
    }

    // Get emit bits, ie the generated event.
    uint8_t dir = state & 0x30;

    if (dir)
    {
        int16_t s = steps; // steps is volatile - save a copy here to avoid multiple fetches
        s = (dir == DIR_CW) ? s + 1 : s - 1;
        if (s > maximum)
            s = minimum;
        else if (s < minimum)
            s = maximum;
        steps = s;
        display.resetBacklightTimer();
    }
}

void RotaryEncoder::setPushed(void)
{
    pushFlag = true;
    display.resetBacklightTimer();
}

bool RotaryEncoder::changed(void)
{
    // returns one if the value changed since the last call of changed.
    static int16_t prevValue = 0;
    int16_t r = read();
    if (r != prevValue)
    {
        prevValue = r;
        return 1;
    }
    if (pushFlag == true)
    {
        return 1;
    }
    return 0;
}
