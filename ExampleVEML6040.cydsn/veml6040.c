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

#include "veml6040.h"
#include "main.h"
#include <project.h>
#include <math.h>
#include <stdio.h>


/*******************************************************************************
* Function Name: VEML6040_startSensor
********************************************************************************
*
* Summary:
*  Send short message to sensor to check wether it responds.
*
* Parameters:
*  None.
*
* Returns:
*  ErrorCode:
*       Error_Code_No_Error = 0
*       Error_Code_Error    = 1
*
*******************************************************************************/

uint8_t VEML6040_checkSensor(void)
{
    uint8_t ErrorCode = I2CM_MasterSendStart(VEML6040_I2C_ADDRESS, I2C_Write);
    I2CM_MasterSendStop();
    if (ErrorCode == I2CM_MSTR_NO_ERROR){
        return Error_Code_No_Error;
    }
    else
    {
        return Error_Code_Error;
    }
}

/*******************************************************************************
* Function Name: VEML6040_setConfiguration
********************************************************************************
*
* Summary:
*  Configure the sensor VEML6040
*
* Parameters:
*  varConfig: Configuration code
*
* Returns:
*  ErrorCode:
*       Error_Code_No_Error = 0
*       Error_Code_Error    = 1
*
*******************************************************************************/

uint8_t VEML6040_setConfiguration(uint8_t varConfig)
{
    uint8_t ErrorCode;
    // start reading
    ErrorCode = I2CM_MasterSendStart(VEML6040_I2C_ADDRESS, I2C_Write);
    if (ErrorCode == I2CM_MSTR_NO_ERROR){ // if no error in transmission then go on
        // send command code + config
        I2CM_MasterWriteByte(COMMAND_CODE_CONF);
        I2CM_MasterWriteByte(varConfig);
        I2CM_MasterSendStop();
        
        lastConfiguration = varConfig;
        
        // everything fine, return
        return Error_Code_No_Error;
    }
    else // abort and return error code
    {
        I2CM_MasterSendStop();
        // else return with error
        return Error_Code_Error;
    }
}

/*******************************************************************************
* Function Name: VEML6040_readSensor
********************************************************************************
*
* Summary:
*  Reads data from sensor.
*
* Parameters:
*  ReturnVal:   Address of variable that should be given back.
*  CommandCode: Defines the color to be read.
*
* Returns:
*  ErrorCode:
*       Error_Code_No_Error = 0
*       Error_Code_Error    = 1
*
*******************************************************************************/

uint8_t VEML6040_readSensor(uint16_t *ReturnVal, uint8_t CommandCode)
{
    // buffer for sensor data
    uint8_t ReadData[2];
    // color data
    uint16_t ColorData;
    // Error
    uint8_t ErrorCode;
    
    // start reading
    ErrorCode = I2CM_MasterSendStart(VEML6040_I2C_ADDRESS, I2C_Write);
    if (ErrorCode == I2CM_MSTR_NO_ERROR) // if everything is okay then proceed
    {
        I2CM_MasterWriteByte(CommandCode);
        // send command code
        I2CM_MasterSendRestart(VEML6040_I2C_ADDRESS, I2C_Read);
        // read data
        ReadData[0] = I2CM_MasterReadByte(I2CM_ACK_DATA);
        ReadData[1] = I2CM_MasterReadByte(I2CM_NAK_DATA);
        I2CM_MasterSendStop();
        
        // combine data (two 8 bit = one 16 bit)
        ColorData = (ReadData[1] << 8) + ReadData[0];
        
        // return data
        *ReturnVal = ColorData;
        
        return Error_Code_No_Error;
    }
    else
    {
        // send stop, otherwise the process won't proceed
        I2CM_MasterSendStop();
        // exit with error code
        return Error_Code_Error;
    }
}

/*******************************************************************************
* Function Name: VEML6040_getRed
********************************************************************************
*
* Summary:
*  Reads red.
*
* Parameters:
*  None.
*
* Returns:
*  Read value of color intensity in counts.
*
*******************************************************************************/

uint16_t VEML6040_getRed()
{
    uint16_t ReturnVal;
    VEML6040_readSensor(&ReturnVal, COMMAND_CODE_RED);
    return ReturnVal;
}

/*******************************************************************************
* Function Name: VEML6040_getGreen
********************************************************************************
*
* Summary:
*  Reads green.
*
* Parameters:
*  None.
*
* Returns:
*  Read value of color intensity in counts.
*
*******************************************************************************/

uint16_t VEML6040_getGreen()
{
    uint16_t ReturnVal;
    VEML6040_readSensor(&ReturnVal, COMMAND_CODE_GREEN);
    return ReturnVal;
}

/*******************************************************************************
* Function Name: VEML6040_getBlue
********************************************************************************
*
* Summary:
*  Reads blue.
*
* Parameters:
*  None.
*
* Returns:
*  Read value of color intensity in counts.
*
*******************************************************************************/

uint16_t VEML6040_getBlue()
{
    uint16_t ReturnVal;
    VEML6040_readSensor(&ReturnVal, COMMAND_CODE_BLUE);
    return ReturnVal;
}

/*******************************************************************************
* Function Name: VEML6040_getWhite
********************************************************************************
*
* Summary:
*  Reads white/unfiltered.
*
* Parameters:
*  None.
*
* Returns:
*  Read value of color intensity in counts.
*
*******************************************************************************/

uint16_t VEML6040_getWhite()
{
    uint16_t ReturnVal;
    VEML6040_readSensor(&ReturnVal, COMMAND_CODE_WHITE);
    return ReturnVal;
}

/*******************************************************************************
* Function Name: VEML6040_getCCT
********************************************************************************
*
* Summary:
*  Calculate CCT from sensor data.
*
* Parameters:
*  None.
*
* Returns:
*  CCT value in Kelvin.
*
*******************************************************************************/

uint16_t VEML6040_getCCT()
{
    float offset = 0.5;
    uint16_t red,blue,green;
    float cct,ccti;
    
    red = VEML6040_getRed();
    green = VEML6040_getGreen();
    blue = VEML6040_getBlue();
    
    // formula in data sheet/application note
    ccti = ((float)red - (float)blue) / (float)green;
    ccti = ccti + offset; 
    cct = 4278.6 * pow(ccti,-1.2455);
    
    return (uint16_t)cct;
}

/*******************************************************************************
* Function Name: VEML6040_getAmbientLight
********************************************************************************
*
* Summary:
*  Calculate ambient light.
*
* Parameters:
*  None.
*
* Returns:
*  Value of ambient light in lux.
*
*******************************************************************************/

float VEML6040_getAmbientLight()
{
    uint16_t sensorValue; 
    float ambientLightInLux;
    
    sensorValue = VEML6040_getGreen();
    
    switch(lastConfiguration & 0x70){
        case VEML6040_IT_40MS:    ambientLightInLux = sensorValue * VEML6040_GSENS_40MS; break;
        case VEML6040_IT_80MS:    ambientLightInLux = sensorValue * VEML6040_GSENS_80MS; break;
        case VEML6040_IT_160MS:   ambientLightInLux = sensorValue * VEML6040_GSENS_160MS; break;
        case VEML6040_IT_320MS:   ambientLightInLux = sensorValue * VEML6040_GSENS_320MS; break;
        case VEML6040_IT_640MS:   ambientLightInLux = sensorValue * VEML6040_GSENS_640MS; break; 
        case VEML6040_IT_1280MS:  ambientLightInLux = sensorValue * VEML6040_GSENS_1280MS; break;   
    default: ambientLightInLux = -1; break;  
    }
    return ambientLightInLux;
}

/*******************************************************************************
* Function Name: startMeasurement
********************************************************************************
*
* Summary:
*  Perform a single measurement.
*
* Parameters:
*  None
*
* Returns:
*  None
*
*******************************************************************************/

void VEML6040_startMeasurement(void){
    // trigger a measurement when in force mode
    VEML6040_setConfiguration(lastConfiguration | VEML6040_TRIG_ENABLE); // take last config and add trigger (OR Operation)
    CyDelay(800);
}