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

#include "project.h"
#include "veml6040.h"

int main(void)
{
    CyGlobalIntEnable; // Enable global interrupts.

    // init LCD component
    LCD_Start();
    LCD_DisplayOn();
    
    // start I2C
    I2CM_Start();
    
    // show start message
    LCD_Position(0,0);
    LCD_PrintString("PSOC 5LP example");
    LCD_Position(1,0);
    LCD_PrintString("for VEML6040");
    CyDelay(1000);
    
    // check if sensor responds
    uint8_t ErrorCode = VEML6040_checkSensor();
    // if error, then show message and wait for sensor
    if (ErrorCode == Error_Code_Error)
    {
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString("Error detected.");
        LCD_Position(1,0);
        LCD_PrintString("Check sensor.");
    }
    // wait for sensor
    while (ErrorCode == Error_Code_Error)
    {
        // repeatedly check if sensor is responding now
        ErrorCode = VEML6040_checkSensor();
        CyDelay(1000);
    }
    
    // show message that sensor is okay
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("Sensor okay.");
    LCD_Position(1,0);
    LCD_PrintString("Proceeding.");
    CyDelay(1000);
    
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("Configuring.");
    LCD_Position(1,0);
    LCD_PrintString("Please wait.");
    CyDelay(1000);

    // configure sensor
    VEML6040_setConfiguration(VEML6040_IT_640MS + VEML6040_TRIG_DISABLE + VEML6040_AF_AUTO + VEML6040_SD_ENABLE);
    CyDelay(1500); // time to process configs
    
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("Everything done.");
    CyDelay(1000);
    
    for(;;)
    {
        // read all three color values
        Red = VEML6040_getRed();
        Green = VEML6040_getGreen();
        Blue = VEML6040_getBlue();
        
        LCD_ClearDisplay();
        // print red value
        LCD_Position(0,0);
        LCD_PrintString("R");
        LCD_Position(1,0);
        LCD_PrintNumber(Red);
        // print green value
        LCD_Position(0,5);
        LCD_PrintString("G");
        LCD_Position(1,5);
        LCD_PrintNumber(Green);
        // print blue value
        LCD_Position(0,10);
        LCD_PrintString("B");
        LCD_Position(1,10);
        LCD_PrintNumber(Blue);
        
        CyDelay(1000);
    }
    
    return 0;
}
