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

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include "./include/driver/wm_gpio.h"
#include "./include/driver/wm_gpio_ex.h"
#include "./include/driver/wm_spi.h"


#include <Arduino.h>
// #include "wm_hal.h"
// SPI_HAS_TRANSACTION means SPI has beginTransaction(), endTransaction(),
// usingInterrupt(), and SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

// SPI_HAS_NOTUSINGINTERRUPT means that SPI has notUsingInterrupt() method
#define SPI_HAS_NOTUSINGINTERRUPT 1

// SPI_ATOMIC_VERSION means that SPI has atomicity fixes and what version.
// This way when there is a bug fix you can check this define to alert users
// of your code if it uses better version of this library.
// This also implies everything that SPI_HAS_TRANSACTION as documented above is
// available too.
#define SPI_ATOMIC_VERSION 1

// Uncomment this line to add detection of mismatched begin/end transactions.
// A mismatch occurs if other libraries fail to use SPI.endTransaction() for
// each SPI.beginTransaction().  Connect an LED to this pin.  The LED will turn
// on if any mismatch is ever detected.
//#define SPI_TRANSACTION_MISMATCH_LED 5

#ifndef LSBFIRST
#define LSBFIRST 0 // Big-endian（大端序）
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

// // fclk = 40MHz / (2 * (div + 1))
// #define SPI_BAUDRATEPRESCALER_2 (0x00000000U)  // 40M / 2 = 20M
// #define SPI_BAUDRATEPRESCALER_4 (0x00000001U)  // 40M / 4 = 10M
// #define SPI_BAUDRATEPRESCALER_8 (0x00000003U)  // 40M / 8 = 5M
// #define SPI_BAUDRATEPRESCALER_10 (0x00000004U) // 40M / 10 = 4M
// #define SPI_BAUDRATEPRESCALER_20 (0x00000009U) // 40M / 20 = 2M
// #define SPI_BAUDRATEPRESCALER_40 (0x00000013U) // 40M / 40 = 1M

#define SPI_CLOCK_DIV2 0
#define SPI_CLOCK_DIV4 1
#define SPI_CLOCK_DIV8 3
#define SPI_CLOCK_DIV16 7
#define SPI_CLOCK_DIV32 15
#define SPI_CLOCK_DIV64 21
#define SPI_CLOCK_DIV128 63

/*
模式0   时钟通常为低电平（CPOL = 0），数据在从低电平到高电平（前沿）（CPHA = 0）的转换时采样。
模式1 - 时钟通常为低电平（CPOL = 0），数据在从高电平到低电平（后沿）（CPHA = 1）的转换时采样。
模式2 - 时钟通常为高电平（CPOL = 1），数据在从高电平到低电平（前沿）（CPHA = 0）的转换时采样。
模式3 - 时钟通常为高电平（CPOL = 1），数据在从低电平到高电平（后沿）（CPHA = 1）的转换时采样。
*/

#define SPI_MODE0 0x00 //default
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

/*Test define*/
#define SPI_LSBFIRST SPI_LITTLEENDIAN
#define SPI_MSBFIRST SPI_BIGENDIAN

#define SPI_TIME_OUT_DEF 100

#ifdef __cplusplus
class SPISettings
{
public:
  SPISettings() : _clock(1000000), _bitOrder(SPI_MSBFIRST), _dataMode(SPI_MODE0) {}
  SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) : _clock(clock), _bitOrder(bitOrder), _dataMode(dataMode) {}
  uint32_t _clock;
  uint8_t _bitOrder;
  uint8_t _dataMode;
};

class SPIClass
{
private:
  // SPI_TypeDef *_spi_num;   //SPI reg base address
   SPI_TypeDef *_spi_id;
  SPI_HandleTypeDef _spi; //SPI Handle
  // int8_t _spi_num;
  // bool _use_hw_ss;
  int8_t _sck;
  int8_t _miso;
  int8_t _mosi;
  int8_t _ss;
  uint32_t _div;
  uint32_t _freq;       /*spi baudrate prescaler*/
  // int8_t _ss_mode;      /*ss mode could use hw_mode or soft_mode*/
  // int8_t _data_mode;    /*data mode could use 4 ways*/
  // int8_t _bit_order;    /*data bit Order LSB or MSB*/

  bool _inTransaction;

   void Error_Handler(void);
  void SPIModeSet(SPI_HandleTypeDef *_spi, uint8_t mode);
  uint8_t SPIModeGet(SPI_HandleTypeDef *_spi);
public:
  //SPIClass(SPI_TypeDef *spi_base = HSPI_BASE);
   SPIClass();
  // SPIClass(int8_t spi_id);
  ~SPIClass();
  void begin(int8_t sck = -1, int8_t miso = -1, int8_t mosi = -1, int8_t ss = -1);
  void end();


  void setClockDivider(uint32_t clockDiv);
  uint32_t getClockDivider();

  void setHwCs(bool use);
  void setBitOrder(uint8_t bitOrder);
  void setDataMode(uint8_t dataMode);
  void setFrequency(uint32_t freq);  
#if 1





  void beginTransaction(SPISettings settings);
  void endTransaction(void);
  void transfer(uint8_t *data, uint32_t size);
  uint8_t transfer(uint8_t data);
  uint16_t transfer16(uint16_t data);
  uint32_t transfer32(uint32_t data);

  void transferBytes(uint8_t *data, uint8_t *out, uint32_t size);
  // void transferBits(uint32_t data, uint32_t *out, uint8_t bits);

  void write(uint8_t data);
  void write16(uint16_t data);
  void write32(uint32_t data);
  void writeBytes( uint8_t *data, uint32_t size);
  // void writePixels( void *data, uint32_t size); //ili9341 compatible
  // void writePattern( uint8_t *data, uint8_t size, uint32_t repeat);
#endif
#if 0
  spi_t *bus() { return _spi; }
  int8_t pinSS() { return _ss; }
#endif  
  // void writePattern_(const uint8_t * data, uint8_t size, uint8_t repeat);

  // static uint8_t initialized;
  // static uint8_t interruptMode; // 0=none, 1=mask, 2=global
  // static uint8_t interruptMask; // which interrupts to mask
  // static uint8_t interruptSave; // temp storage, to restore state
  // static SPI_InitTypeDef spi_init; // = {SPI_MODE_MASTER,  SPI_POLARITY_LOW, SPI_PHASE_1EDGE, SPI_NSS_SOFT, SPI_CLOCK_DIV8, SPI_LITTLEENDIAN};
  // static SPI_HandleTypeDef hspi;
};

extern SPIClass SPI;
#endif
#endif

