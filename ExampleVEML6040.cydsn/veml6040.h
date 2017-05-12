/*
MIT License

Copyright (c) 2017 JSchfer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef veml6040_h
#define veml6040_h

#include <project.h>
#include "main.h"
    
/*******************************************************************************/
// VEML6040
// VEML6040 I2C ADDRESS
#define VEML6040_I2C_ADDRESS   0x10

// integration time, more sensitive when high
#define VEML6040_IT_40MS       0x00
#define VEML6040_IT_80MS       0x10
#define VEML6040_IT_160MS      0x20
#define VEML6040_IT_320MS      0x30
#define VEML6040_IT_640MS      0x40
#define VEML6040_IT_1280MS     0x50

// trigger measurement when in force mode
#define VEML6040_TRIG_DISABLE  0x00
#define VEML6040_TRIG_ENABLE   0x04
    
// Auto/Force (auto: sensor measures automatically, force: measurement needs to be triggered)
#define VEML6040_AF_AUTO       0x00
#define VEML6040_AF_FORCE      0x02

// enable/disable sensor
#define VEML6040_SD_ENABLE     0x00
#define VEML6040_SD_DISABLE    0x01 

// command codes
#define COMMAND_CODE_CONF      0x00
#define COMMAND_CODE_RED       0x08
#define COMMAND_CODE_GREEN     0x09
#define COMMAND_CODE_BLUE      0x0A
#define COMMAND_CODE_WHITE     0x0B

// G SENSITIVITY, see also function VEML6040_getAmbientLight
#define VEML6040_GSENS_40MS       0.25168
#define VEML6040_GSENS_80MS       0.12584
#define VEML6040_GSENS_160MS      0.06292
#define VEML6040_GSENS_320MS      0.03146
#define VEML6040_GSENS_640MS      0.01573
#define VEML6040_GSENS_1280MS     0.007865
/*******************************************************************************/

// I2C
#define I2C_Write   0x00
#define I2C_Read    0x01
    
// Error Codes
#define Error_Code_No_Error 0x00
#define Error_Code_Error 0x01

/*******************************************************************************/

// private
uint8_t VEML6040_readSensor(uint16_t *ReturnVal, uint8_t CommandCode);
uint8_t lastConfiguration;

// public
uint8_t VEML6040_checkSensor(void); // check if sensor responds
uint8_t VEML6040_setConfiguration(uint8_t); // send config to sensor
uint16_t VEML6040_getRed(); // receive red value
uint16_t VEML6040_getGreen(); // receive green value
uint16_t VEML6040_getBlue(); // receive blue value
uint16_t VEML6040_getWhite(); // receive white (unfiltered) value
uint16_t VEML6040_getCCT(); // receive correlated color temperature value
float VEML6040_getAmbientLight(); // receive ambient light value
void VEML6040_startMeasurement(void); // trigger a single measurement when in force mode

#endif
