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

// Included in this version of the BrewPi Firmware courtesy of
// https://github.com/slintak. Thank you for coding this, and thank
// you for reaching out on HomeBrewTalk!

// YWROBOT
// last updated on 21/12/2011
// Tim Starling Fix the reset bug (Thanks Tim)
// wiki doc http://www.dfrobot.com/wiki/index.php?title=I2C/TWI_LCD1602_Module_(SKU:_DFR0063)
// Support Forum: http://www.dfrobot.com/forum/
// Compatible with the Arduino IDE 1.0
// Library version:1.1

#include "I2cLcd.h"

#include "Brewpi.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <inttypes.h>
#include "Arduino.h"

extern "C" {
#include <Wire.h>
}

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

IIClcd::IIClcd(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows)
{
    _Addr = lcd_Addr;  // This now gets ignored
    _cols = lcd_cols;
    _rows = lcd_rows;
    _backlightval = LCD_NOBACKLIGHT;
}

void IIClcd::scan_address() {
//	Wire.begin();
    for (byte i = 8; i < 120; i++)
    {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0)
        {
            // We found the i2c device address.
            _Addr = i;
            i = 120;
            delay(1);
        }
    }
}

void IIClcd::init() {
    init_priv();
    _backlightTime = 0;
}

void IIClcd::init_priv()
{
#if defined(ESP8266) || defined(ESP32)
    Wire.begin(IIC_SDA, IIC_SCL);
	scan_address();
#else
    Wire.begin();
    scan_address();
#endif
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    begin(_cols, _rows);
}

void IIClcd::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
    if (lines > 1) {
        _displayfunction |= LCD_2LINE;
    }
    _numlines = lines;
    _currline = 0;
    _currpos = 0;

    // For some 1 line displays you can select a 10 pixel high font
    if ((dotsize != 0) && (lines == 1)) {
        _displayfunction |= LCD_5x10DOTS;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // According to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
    delay(50);

    // Now we pull both RS and R/W low to begin commands
    expanderWrite(_backlightval);	// Reset expanderand turn backlight off (Bit 8 =1)
    delay(1000);

    // Put the LCD into 4 bit mode
    // This is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // We start in 8bit mode, try to set 4 bit mode
    write4bits(0x03 << 4);
    delayMicroseconds(4500); // wait min 4.1ms

    // Second try
    write4bits(0x03 << 4);
    delayMicroseconds(4500); // wait min 4.1ms

    // Third go!
    write4bits(0x03 << 4);
    delayMicroseconds(150);

    // Finally, set to 4-bit interface
    write4bits(0x02 << 4);

    // Set # lines, font size, etc.
    command(LCD_FUNCTIONSET | _displayfunction);

    // Turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // Clear it off
    clear();

    // Initialize to default text direction (for roman languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // Set the entry mode
    command(LCD_ENTRYMODESET | _displaymode);

    home();
}

/**********
 * High level commands for the user
 */

void IIClcd::clear() {
    command(LCD_CLEARDISPLAY); // Clear display, set cursor position to zero

    for (uint8_t i = 0; i < _rows; i++) {
        for (uint8_t j = 0; j < _cols; j++) {
            content[i][j] = ' '; // Initialize on all spaces
        }
        content[i][_cols] = '\0'; // NULL terminate string
    }

    delayMicroseconds(2000);  // This command takes a long time
}

void IIClcd::home() {
    command(LCD_RETURNHOME);  // Set cursor position to zero
    delayMicroseconds(2000);  // This command takes a long time
}

void IIClcd::setCursor(uint8_t col, uint8_t row) {
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row > _numlines) {
        row = _numlines - 1;  // Count rows starting w/0
    }

    _currline = row;
    _currpos = col;
    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void IIClcd::noDisplay() {
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void IIClcd::display() {
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void IIClcd::noCursor() {
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void IIClcd::cursor() {
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void IIClcd::noBlink() {
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void IIClcd::blink() {
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void IIClcd::scrollDisplayLeft(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void IIClcd::scrollDisplayRight(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void IIClcd::leftToRight(void) {
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void IIClcd::rightToLeft(void) {
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void IIClcd::autoscroll(void) {
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void IIClcd::noAutoscroll(void) {
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void IIClcd::createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i<8; i++) {
        write(charmap[i]);
    }
}

// Turn the (optional) backlight off/on
void IIClcd::noBacklight(void) {
    _backlightval = LCD_NOBACKLIGHT;
    expanderWrite(0);
}

void IIClcd::backlight(void) {
    _backlightval = LCD_BACKLIGHT;
    expanderWrite(0);
}

/*********** 
 * Mid level commands, for sending data/cmds
 */

inline void IIClcd::command(uint8_t value) {
    send(value, 0);
}

inline size_t IIClcd::write(uint8_t value) {
    content[_currline][_currpos] = value;
    _currpos++;
    if (!_bufferOnly) {
        send(value, Rs);
    }
    return 0;
}

/************
 * Low level data pushing commands
 */

// Write either command or data
void IIClcd::send(uint8_t value, uint8_t mode) {
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;
    write4bits((highnib) | mode);
    write4bits((lownib) | mode);
}

void IIClcd::write4bits(uint8_t value) {
    expanderWrite(value);
    pulseEnable(value);
}

void IIClcd::expanderWrite(uint8_t _data) {
    uint8_t data = ((uint8_t)(_data) | _backlightval);
//	twi_writeTo(_Addr, &data, 1, true, true);
    Wire.beginTransmission(_Addr);
    Wire.write(data);
    Wire.endTransmission();
}

void IIClcd::pulseEnable(uint8_t _data) {
    expanderWrite(_data | En);	// En high
    delayMicroseconds(1);		// Enable pulse must be >450ns

    expanderWrite(_data & ~En);	// En low
    delayMicroseconds(50);		// Commands need > 37us to settle
}

// This resets the backlight timer and updates the SPI output
void IIClcd::resetBacklightTimer(void) {
    _backlightTime = ticks.seconds();
}

void IIClcd::updateBacklight(void) {
    // True = OFF, False = ON
#if BACKLIGHT_AUTO_OFF_PERIOD > 0 || BREWPI_SIMULATE
    bool backLightOutput = BREWPI_SIMULATE || ticks.timeSince(_backlightTime) > BACKLIGHT_AUTO_OFF_PERIOD;
#elif defined(ESP8266) || defined(ESP32)
    bool backLightOutput = toggleBacklight;
#else
    // Default to backlight always on
    bool backLightOutput = false;
#endif
    if (backLightOutput) {
        noBacklight();
    } else {
        backlight();
    }
}

// Puts the content of one LCD line into the provided buffer.
void IIClcd::getLine(uint8_t lineNumber, char * buffer) {
    const char* src = content[lineNumber];
    for (uint8_t i = 0; i < _cols; i++) {
        char c = src[i];
        buffer[i] = (c == 0b11011111) ? 0xB0 : c;
    }
    buffer[_cols] = '\0'; // NULL terminate string
}

void IIClcd::printSpacesToRestOfLine(void) {
    while (_currpos < _cols) {
        write(' ');
    }
}

#ifndef print_P_inline
void IIClcd::print_P(const char * str) { // Print a string stored in PROGMEM
    for(uint8_t i=0; i<strlen_P(str); i++) {
        write(pgm_read_byte_near(str+i));
    }
}

void IIClcd::print(char * str) { // Print a string stored in PROGMEM
    for(uint8_t i=0; i<strlen(str); i++) {
        write(str[i]);
    }
}
#endif
