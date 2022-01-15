/*
  wiring_digital.c - digital input and output functions
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

#define ARDUINO_MAIN
#include "Arduino.h"
#include "pins_arduino.h"
#include "./include/driver/wm_gpio.h"

void pinMode(uint8_t pin, uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_TypeDef *port = (pin > 15)? GPIOB:GPIOA;
    if (pin > 15) { pin -= 16; }

    GPIO_InitStruct.Pin = 1<<pin;
    switch (mode)
    {
        case INPUT:
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            break;
        case INPUT_PULLUP:
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            break;
        case INPUT_PULLDOWN:
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            break;
        case OUTPUT:
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            break;
        default:
            break;
    }

    HAL_GPIO_Init(port, &GPIO_InitStruct);
}
/*
static void __turnOffPWM(uint8_t timer)
{

}
*/
void digitalWrite(uint8_t pin, uint8_t val)
{
    GPIO_TypeDef *port = (pin > 15)? GPIOB:GPIOA;
    if (pin > 15) { pin -= 16; }
    HAL_GPIO_WritePin(port, 1 << pin, (GPIO_PinState)val);
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (GPIO_PinState)val);
}

uint8_t digitalRead(uint8_t pin)
{
    GPIO_TypeDef *port = (pin > 15)? GPIOB:GPIOA;
    if (pin > 15) { pin -= 16; }
    return HAL_GPIO_ReadPin(port, 1<<pin);
}

void digitalToggle(uint8_t pin)
{
    GPIO_TypeDef *port = (pin > 15)? GPIOB:GPIOA;
    if (pin > 15) { pin -= 16; }
    HAL_GPIO_TogglePin(port, 1<<pin);
}
