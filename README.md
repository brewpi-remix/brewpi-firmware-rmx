# ![BrewPi Legacy Remix Logo](https://raw.githubusercontent.com/lbussy/brewpi-www-rmx/master/brewpi_logo.png)
*[@LBussy](https://github.com/lbussy)'s forks of the original [BrewPi Project](https://github.com/BrewPi)*

This project contains the firmware for [BrewPi Legacy Remix](https://www.brewpi.com/this-is-brewpi-0-2/) which runs on a [Raspberry Pi](https://www.raspberrypi.org/), communicating with an [Arduino](https://www.arduino.cc/en/guide/introduction).  Despite the original creators no longer actively supporting BrewPi on Arduino, and despite the Arduino being arguably one of the least capable [controllers](https://en.wikipedia.org/wiki/Controller_(computing)) on the market, BrewPi on Arduino still has an amazing following among home brewers.  When I last checked there were [7473 posts in this thread](https://www.homebrewtalk.com/forum/threads/howto-make-a-brewpi-fermentation-controller-for-cheap.466106/) on [HomeBrewTalk.com](https://www.homebrewtalk.com/) since March 19, 2014 when [@FuzzeWuzze](https://www.homebrewtalk.com/forum/members/fuzzewuzze.123340/) started the thread.

These [forks](https://en.wikipedia.org/wiki/Fork_(software_development)) are intended for the fans of the *original* BrewPi (called "Legacy" in BrewPi circles).  For current [BrewPi Spark 3](https://www.brewpi.com/) information and support, please continue to use and support [the original project](https://github.com/BrewPi) which by all accounts is far more capable.

Before we proceed, a huge thank you to [Elco Jacobs](https://github.com/elcojacobs), without whom none of this would be possible.
# Getting Started

To get started, please head over to the [BrewPi Legacy Remix tools](https://github.com/lbussy/brewpi-tools-rmx/blob/master/README.md) README page, and follow along there.

## Building the Firmware for Arduino
*Not for the faint of heart ...*

End users will *not* have to compile the firmware themselves.  Pre-compiled binaries may be found [in releases](https://github.com/lbussy/brewpi-firmware-rmx/releases), however they are automatically made available and applied during the normal BrewPi Legacy Remix install process.

To build the firmware for Arduino, you will have to use [Atmel Studio version 6.2](https://www.microchip.com/mplab/avr-support/avr-and-sam-downloads-archive). The repo includes an Atmel studio project. Select the right processor (Atmega328P for the Arduino Uno, Atmega32U4 for the Leonardo) and set up the right shield in Config.h.
