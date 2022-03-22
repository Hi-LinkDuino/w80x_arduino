/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SPI.h"

SPIClass SPI;

uint8_t SPIClass::initialized = 0;
uint8_t SPIClass::interruptMode = 0;
uint8_t SPIClass::interruptMask = 0;
uint8_t SPIClass::interruptSave = 0;
SPI_InitTypeDef SPIClass::spi_init  = {SPI_MODE_MASTER,  SPI_POLARITY_LOW, SPI_PHASE_1EDGE, SPI_NSS_SOFT, SPI_CLOCK_DIV8, SPI_LITTLEENDIAN};
SPI_HandleTypeDef SPIClass::hspi = {};


void SPIClass::begin()
{
    noInterrupts(); // Protect from a scheduler and prevent transactionBegin
    if (!initialized) {
        __HAL_RCC_SPI_CLK_ENABLE();
        __HAL_AFIO_REMAP_SPI_CS(GPIOB, GPIO_PIN_4);   // GPIO init
        __HAL_AFIO_REMAP_SPI_CLK(GPIOB, GPIO_PIN_2);
        __HAL_AFIO_REMAP_SPI_MISO(GPIOB, GPIO_PIN_3);
        __HAL_AFIO_REMAP_SPI_MOSI(GPIOB, GPIO_PIN_5);
        hspi.Instance = SPI_HAL;
        hspi.Init = spi_init;

        if (HAL_SPI_Init(&hspi) != HAL_OK)
        {
           // Error_Handler();
        }
    }
    initialized++; // reference count

}

void SPIClass::end() {
  noInterrupts(); // Protect from a scheduler and prevent transactionBegin
  // Decrease the reference counter
  if (initialized)
    initialized--;
  // If there are no more references disable SPI
  if (!initialized) {
    __HAL_RCC_SPI_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
  }
}

void SPIClass::usingInterrupt(uint8_t interruptNumber)
{

}

void SPIClass::notUsingInterrupt(uint8_t interruptNumber)
{

}
