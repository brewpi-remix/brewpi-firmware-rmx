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

import subprocess
Import("env")
from pprint import pprint as pp

# Get framework
framework = ""
try:
    framework = env["PIOFRAMEWORK"][0]
except:
    framework = "unknown"

# Get board
board = ""
try:
    board = str(env["BOARD"])
except:
    board = "unknown"

# Get 0.0.0 version from latest Git tag
tag = ""
tagcmd = "git describe --tags --abbrev=0"
try:
    version = subprocess.check_output(tagcmd, shell=True).decode().strip()
except:
    pass
if version and version is not None:
    tag = version
else:
    tag = "0.0.0"

# Get shield
shield = "unknown"
#
# Check first in PIO defines
my_flags = env.ParseFlags(env['BUILD_FLAGS'])
dict = {k: v for (k, v) in my_flags.get("CPPDEFINES")}
shieldType = dict.get('BREWPI_STATIC_CONFIG')
if shieldType and shieldType is not None:
    shield = shieldType.split("_")[-1].lower()

# If none in defines, get shield from Config.h
if shield == "unknown":
    shields = []
    linenum = 0
    define = "#define BREWPI_STATIC_CONFIG".lower()
    try:
        with open (str(env["PROJECT_INCLUDE_DIR"]) + '/Config.h', 'rt') as config:
            for line in config:
                linenum += 1
                if line.lower().startswith(define):
                    shields.append(line.rstrip('\n'))
        for variant in shields:
            configShield = variant.split(" ")[-1].split("_")[-1].lower()
        if configShield and configShield is not None:
            shield = configShield
    except:
        pass

firmwareName = "brewpi-%s-%s-%s-%s" % (
    framework,
    board,
    shield,
    tag)

env.Replace(PROGNAME=firmwareName)
