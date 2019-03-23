## 0.2.11
The primary change in this release is to move the workspace to PlatformIO from Atmel Studio.

### Features
 - Move to Platformio
 - Remove (some of) the Spark shrapnel that was in 0.2.10
 - Bump logMessage.h to be compatible with @Thorrak's ESP8266 firmware

### Bugfixes
 - Added a reset to help LCD scramble
 - Re-establish build version in eeprom (remove "unknown")

## 0.2.10a
### Bugfixes
This was the version provided after the move from the original BrewPi repositories.  The [0.2.10 tag in that repository](https://github.com/BrewPi/firmware/releases/tag/0.2.10) was correct/complete, however the [HEAD of the Legacy branch](https://github.com/BrewPi/firmware/tree/legacy) included some commits which broke the Arduino code.  These were rolled back to establish a known good starting point.  In addition the following fixes were merged:
- [Fix for bug in setting calibration offset ](https://github.com/BrewPi/firmware/commit/41ccbd502540cad60fdeb5a82e3b6e14444cafbe)
- [Fixed ncorrect [sic] capitalization (BrewPi.h instead of Brewpi.h) in OneWire.h](https://github.com/BrewPi/firmware/commit/c0a4cef2df38bb206163fda964ddfa48907b0c10)

I missed a test case in yesterday's release. Converting a temperature very close to a whole number (e.g. 20.998) converted 1 degree too high (22) due to erroneous rounding of an internal result. This releases fixes the bug and improves the unit tests to go through the entire temperature range to make sure it works.

## 0.2.10
### Bugfixes
I missed a test case in yesterday's release. Converting a temperature very close to a whole number (e.g. 20.998) converted 1 degree too high (22) due to erroneous rounding of an internal result. This releases fixes the bug and improves the unit tests to go through the entire temperature range to make sure it works.

## 0.2.9
This is a faster release in between our normal release schedule to fix some bugs that we thought were bad enough to warrant a hotfix release.

### Features
No new features in this release

### Bugfixes
- ####Temperature filter initialization
Temperature sensor filters could be used uninitialized in when a fridge or beer sensor was plugged in 1-60 seconds after start. See issue #18.

- ####External flash
A bug in the flashee library, which handles external flash to store settings, could cause a hard reset and temp sensors to be installed with an incorrect device address. This would only happen when a flash was worn out and the new data block spanned multiple pages, which is unlikely to have occured yet under normal use. See https://github.com/m-mcgowan/spark-flashee-eeprom/issues/16.

- ####Error checking when parsing strings received from script
When an invalid temperature or setting string was received from the script, the value defaulted to 0. We have refactored the string parsing functions to detect and ignore invalid strings. See issue #16 and #21.

- ####Small rounding errors in conversions
We fixed small 1 bit rounding errors (0.0002 degree) in or temperature conversion functions. See issue #22.


### Enhancements
[Catch](https://github.com/philsquared/Catch) unit tests were added to the build (replacing googletest). We used it to test all the new temperature conversion functions. Closes issue #15.


## 0.2.8

### Features

- [Brewpi Spark] New Startup screen! with tap anywhere to recalibrate touch.
- [Brewpi Spark] New Controller screen! view temperatures, mode and state. Watch these change as you make changes in the web UI. 
- [BrewPi Spark] Controller now automatically switches between the controller screen and the test screen based on mode. If you set it to test mode via the web UI (under advanced settings), the test screen will be shown. Otherwise, the main controller screen.

### Enhancements

- Heater is now active together with the light when Light as Heater is selected.
- Arduino supported with this release.
- Removed 'T' command (calibrate) that was causing issues.
- Default beer and fridge setting is now --.- (disabled). This aligns with the default mode OFF.


### Under the Hood

- Better temperature sensor initialization and disconnect detection (faster and more reliable)
- Increased OneWire reliability
- [Arduino] OneWire class refactored
- [Arduino] Updated Arduino dependencies. This should increase Serial reliability on the Leonardo and fix dropped characters
- Fixed bug that timer was shown in OFF mode
- Use git describe to generate build name

<!--stackedit_data:
eyJoaXN0b3J5IjpbMTE3Mzk5NzUwOF19
-->