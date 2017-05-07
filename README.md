# veml6040
Library for RGBW color sensor on PSOC 5LP

## General stuff
This example program is written with PSOC Creator. The hardware used is:
- CY8CKIT-059 PSOC 5LP Prototyping Kit
- VEML6040: RGBW color sensor with I2C interface
- standard LCD

Warning: Be careful, when you program the PSOC. Always disconnect the PSOC from the sensor while programming. The sensor can only stand up to 3.6 V, so the 5 V from the USB connection could kill it.

## How to use

You can copy only the files **veml6040.h** and **veml6040.c** into your project to use the sensor. The I2C element in TopDesign is named **ICM**.

### Summary of the funcionality
Function name             |    Function purpose
--------------------------|---------------
VEML6040_checkSensor      | Check if sensor responds. If yes a 0 will be given back, otherwise a 1.
VEML6040_setConfiguration | Configure sensor. For further information read data sheet.
VEML6040_getRed           | Returns the value for red light intensity.
VEML6040_getGreen         | Returns the value for green light intensity.
VEML6040_getBlue          | Returns the value for blue light intensity.
VEML6040_getWhite         | Returns the value for white (= unfiltered) light intensity.
VEML6040_getCCT           | Returns the correlated color temperature.
VEML6040_getAmbientLight  | Returns the ambient light in lux.

There are other functions which are used mainly in background.

## Author
Jeroen Schäfer

## License
This project is licensed under the MIT License - see the LICENSE.md file for details.

## Acknowledgements
The library content is partially taken from [VEML6040](https://github.com/thewknd/VEML6040.git) by @thewknd and altered to work with PSOC 5LP.
