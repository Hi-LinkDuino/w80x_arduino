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

typedef enum
{
    ADC_UNINT = 0,
    ADC_INIT
} adcChannelStateTypeDef;

typedef struct
{
    uint8_t ch;
    uint8_t ch_state;
    ADC_HandleTypeDef hadc;
} adcPinStateTypeDef;

adcPinStateTypeDef adcPinState[5] = {0};
ADC_HandleTypeDef hadc1;

void Error_Handler(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

const uint32_t channel[] = {
    ADC_ANA_CR_CH_0,
    ADC_ANA_CR_CH_3,
    ADC_ANA_CR_CH_2,
    ADC_ANA_CR_CH_1};

int analogRead(uint8_t pin)
{
    uint32_t val = 0;
    uint8_t ch = 0;
    if ((pin >= PA1) && (pin <= PA4)) /*TODO IO校验和传递*/
    {
        ch = pin - 1;
        adcPinState->ch = ch;
        if (adcPinState[ch].ch_state == ADC_UNINT) /*This pin is not in adc mode,init first*/
        {
            printf("pin [%d] init \r\n", pin);
            adcPinState[ch].ch_state = ADC_INIT;
            adcPinState[ch].hadc.Instance = ADC;
            adcPinState[ch].hadc.Init.channel = channel[ch];
            adcPinState[ch].hadc.Init.freq = 1000;
            HAL_ADC_Init(&(adcPinState[ch].hadc));
        }

        val = HAL_ADC_GET_INPUT_VOLTAGE(&adcPinState[ch].hadc);
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

__attribute__((isr)) void ADC_IRQHandler(void)
{
    uint8_t i = 0;

    for (i = 0; i < 4; i++)
    {
        if (adcPinState[i].ch_state == ADC_INIT)
        {
            HAL_ADC_IRQHandler(&adcPinState[i].hadc);
        }
    }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC)
    {
        __HAL_RCC_ADC_CLK_ENABLE();
        __HAL_RCC_GPIO_CLK_ENABLE();

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

        // 如果用到中断方式需要使能中断
        HAL_NVIC_SetPriority(ADC_IRQn, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    __HAL_RCC_ADC_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1);
    HAL_NVIC_DisableIRQ(ADC_IRQn);
}
