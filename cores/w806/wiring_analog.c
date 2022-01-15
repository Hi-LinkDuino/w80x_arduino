/*
  wiring_analog.c - analog input and output
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 28 September 2010 by Mark Sproul

  $Id: wiring.c 248 2007-02-03 15:36:30Z mellis $
*/

//#include "wiring_private.h"
#include "pins_arduino.h"
#include "./include/driver/wm_adc.h"

#if 0
// int analogRead(uint8_t);
// void analogReference(uint8_t mode);
// void analogWrite(uint8_t, uint16_t);
// void analogReadResolution(uint8_t);

typedef enum{
    ADC_UNINT = 0,
    ADC_INIT
};


typedef struct{
    uint8_t pin;
    uint8_t pin_state;
    ADC_HandleTypeDef hadc;
}adcPinStateTypeDef;


static adcPinStateTypeDef adcPinState[4] = {0};

// static char *adc_pin_mode(uint8_t pin)
// {


// }
void Error_Handler(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
int analogRead(uint8_t pin)
{
    int val = 0;
    // if((pin >= PA1) && (pin <= PA4))                /*TODO IO校验和传递*/       
    // {
    //     if(adcPinState[pin].pin_state == ADC_UNINT)         /*This pin is not in adc mode,init first*/
    //     {
    //         adcPinState[pin].hadc.Instance = ADC;
    //         adcPinState[pin].hadc.Init.channel = (pin << ADC_ANA_CR_CH_Pos);
    //         adcPinState[pin].hadc.Init.freq = 1000;   
    //         HAL_ADC_Init(&adcPinState[pin].hadc);

    //         adcPinState[pin].pin_state = ADC_INIT;
    //     }

    //     val =  HAL_ADC_GET_INPUT_VOLTAGE(&adcPinState[pin].hadc);
    // }
    static char flag = 0;
    static ADC_HandleTypeDef hadc;
    if(!flag)
    {
        flag = 1;
	hadc.Instance = ADC;
	hadc.Init.channel = ADC_CHANNEL_0;
	hadc.Init.freq = 1000;
	if (HAL_ADC_Init(&hadc) != HAL_OK)
	{
    
		Error_Handler();
	}
        // if (HAL_ADC_Init(&hadc) != HAL_OK)
        // {
        //     Error_Handler();
        // }
    }
    
   // val =  HAL_ADC_GET_INPUT_VOLTAGE(&hadc);


    return val;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    
}

void Error_Handler(void)
{
    
	while (1)
	{
    
	}
}

#endif