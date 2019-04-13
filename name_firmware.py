#!/usr/bin/python

# Copyright (C) 2018, 2019 Lee C. Bussy (@LBussy)

# This file is part of LBussy's BrewPi Firmware Remix (BrewPi-Firmware-RMX).
#
# BrewPi Firmware RMX is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# BrewPi Firmware RMX is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with BrewPi Firmware RMX. If not, see <https://www.gnu.org/licenses/>.

# These Firmwares were originally a part of brewpi-Firmware, a part of
# the BrewPi project. Legacy support (for the very popular Arduino
# controller) seems to have been discontinued in favor of new hardware.

# All credit for the original brewpi-Firmware goes to @elcojacobs,
# @m-mcgowan, @rbrady, @steersbob, @glibersat, @Niels-R and I'm sure
# many more contributors around the world. My apologies if I have
# missed anyone; those were the names listed as contributors on the
# Legacy branch.

# See: 'original-license.md' for notes about the original project's
# license and credits. */

import subprocess
Import("env")

my_flags = env.ParseFlags(env['BUILD_FLAGS'])
defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}

# Parse out defined shield from Config.h
shields = []
linenum = 0
define = "#define BREWPI_STATIC_CONFIG".lower()
with open (str(env["PROJECTINCLUDE_DIR"]) + '/Config.h', 'rt') as config:
    for line in config:
        linenum += 1
        if line.lower().startswith(define):
            shields.append(line.rstrip('\n'))
for variant in shields:
    shield = variant.split(" ")[-1].split("_")[-1].lower()

env.Replace(PROGNAME="brewpi-%s-%s-%s-%s" % (
    env["PIOFRAMEWORK"][0],
    str(env["BOARD"]),
    shield,
    defines.get("PIO_SRC_TAG")))
