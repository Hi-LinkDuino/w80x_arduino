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

#include "pins_arduino.h"
#include "./include/driver/wm_adc.h"

#define ADC_NORMAL (0U) /*polling mode*/
#define ADC_IT (1U)     /*interrupt mode*/

#define ADC_MODE ADC_NORMAL /*using polling mode */
#define ADC_DEFAULT_OFFSET 176859
#define ADC_MAX_VAL 270000

typedef struct
{
    uint8_t ch;
    uint8_t pin;
    //uint8_t ch_state;
    ADC_HandleTypeDef hadc;
} adcPinStateTypeDef;

adcPinStateTypeDef adcPinState[ADC_COUNT] = {0};

void Error_Handler(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

// void analogReference(uint32_t val)
// {
//     uint8_t ch = 0;
//     if (val == 0)
//     {
//         val = ADC_DEFAULT_OFFSET;
//     }

//     for (ch = 0; ch < sizeof(ADC_COUNT); ch++)
//     {
//         adcPinState[ch].hadc.offset = val;
//     }
// }

int analogRead(uint8_t pin) /*all ADC channel range is 0-2.4V*/
{
    uint32_t val = 0;
    uint8_t channel_index = 0;

    if (g_APinDescription[pin].ulPinAttribute & PIN_ATTR_ANALOG)
    {
        channel_index = (uint16_t)g_APinDescription[pin].ulAnalogChannel >> 8; //ADC_ANA_CR_CH_0

        if (g_pinStatus[pin] != PIN_ATTR_ANALOG)
        {
            g_pinStatus[pin] = PIN_ATTR_ANALOG;

            adcPinState[channel_index].pin = pin;
            adcPinState[channel_index].hadc.Instance = ADC;
            adcPinState[channel_index].hadc.Init.channel = g_APinDescription[pin].ulAnalogChannel; //channel[ch];
            adcPinState[channel_index].hadc.Init.freq = 1000;
            adcPinState[channel_index].hadc.offset = ADC_DEFAULT_OFFSET;
            HAL_ADC_Init(&(adcPinState[channel_index].hadc));
        }

        assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

#if (ADC_MODE == ADC_IT)
        HAL_ADC_Start_IT(&adcPinState[ch].hadc);
#else
        HAL_ADC_Start(&adcPinState[channel_index].hadc);
        HAL_ADC_PollForConversion(&adcPinState[channel_index].hadc);
        HAL_ADC_Stop(&adcPinState[channel_index].hadc);

        val = ADC->DR;
        val = val - ADC_DEFAULT_OFFSET;
        if ((val < 0) || (val > ADC_MAX_VAL))
        {
            val = 0;
        }
#endif
    }

    return val;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
}

void Error_Handler(void)
{
    while (1)
    {
    }
}

#if (ADC_MODE == ADC_IT)
__attribute__((isr)) void ADC_IRQHandler(void)
{
    uint8_t i = 0;

    for (i = 0; i < 4; i++)
    {
        if (adcPinState[i].hadc != NULL)
        {
            HAL_ADC_IRQHandler(&adcPinState[i].hadc);
        }
    }
}
#endif

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC)
    {
        __HAL_RCC_ADC_CLK_ENABLE();
        // __HAL_RCC_GPIO_CLK_ENABLE();

        //ADC_CHANNEL_0 : PA1
        //ADC_CHANNEL_1 : PA4
        //ADC_CHANNEL_2 : PA3
        //ADC_CHANNEL_3 : PA2
        //ADC_CHANNEL_0_1 : PA1 and PA4
        //ADC_CHANNEL_2_3 : PA3 and PA2
        if (hadc->Init.channel == ADC_CHANNEL_0)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_1);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_1)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_4);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_2)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_3);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_3)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_2);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_0_1)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_1);
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_4);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_2_3)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_3);
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_2);
        }
#if (ADC_MODE == ADC_IT)
        // 如果用到中断方式需要使能中断
        HAL_NVIC_SetPriority(ADC_IRQn, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
#endif
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    __HAL_RCC_ADC_CLK_DISABLE();

    for (int i = 0; i < ADC_COUNT; i++)
    {
        if (hadc->Init.channel == adcPinState[i].hadc.Init.channel)
        {
            if (g_pinStatus[adcPinState[i].pin] == PIN_ATTR_ANALOG)
            {
                g_pinStatus[adcPinState[i].pin] = PIN_ATTR_NONE;
                HAL_GPIO_DeInit(GPIOA, adcPinState[i].pin);
                break;
            }
        }
    }
#if (ADC_MODE == ADC_IT)
    HAL_NVIC_DisableIRQ(ADC_IRQn);
#endif
}
