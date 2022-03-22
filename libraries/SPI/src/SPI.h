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

#include <Arduino.h>

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
#define LSBFIRST 0  // Big-endian（大端序）
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif


#define SPI_BAUDRATEPRESCALER_10        (0x00000004U)	// 40M / 10 = 4M
#define SPI_BAUDRATEPRESCALER_20        (0x00000009U)	// 40M / 20 = 2M
#define SPI_BAUDRATEPRESCALER_40        (0x00000013U)	// 40M / 40 = 1M

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

#define SPI_MODE0 0x00  //default
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

class SPISettings {
public:
  SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
    if (__builtin_constant_p(clock)) {
      init_AlwaysInline(clock, bitOrder, dataMode);
    } else {
      init_MightInline(clock, bitOrder, dataMode);
    }
  }
  SPISettings() {
    init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0);
  }
private:
  void init_MightInline(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
    init_AlwaysInline(clock, bitOrder, dataMode);
  }
  void init_AlwaysInline(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
    __attribute__((__always_inline__)) {
    // Clock settings are defined as follows. Note that this shows SPI2X
    // inverted, so the bits form increasing numbers. Also note that
    // fosc/64 appears twice
    // SPR1 SPR0 ~SPI2X Freq
    //   0    0     0   fosc/2
    //   0    0     1   fosc/4
    //   0    1     0   fosc/8
    //   0    1     1   fosc/16
    //   1    0     0   fosc/32
    //   1    0     1   fosc/64
    //   1    1     0   fosc/64
    //   1    1     1   fosc/128

    // We find the fastest clock that is less than or equal to the
    // given clock rate. The clock divider that results in clock_setting
    // is 2 ^^ (clock_div + 1). If nothing is slow enough, we'll use the
    // slowest (128 == 2 ^^ 7, so clock_div = 6).
    uint8_t clockDiv;

    // When the clock is known at compiletime, use this if-then-else
    // cascade, which the compiler knows how to completely optimize
    // away. When clock is not known, use a loop instead, which generates
    // shorter code.
    if (__builtin_constant_p(clock)) {
      if (clock >= SPI_FCLK / 2) {
        clockDiv = SPI_CLOCK_DIV2;
      } else if (clock >= SPI_FCLK / 4) {
        clockDiv = SPI_CLOCK_DIV4;
      } else if (clock >= SPI_FCLK / 8) {
        clockDiv = SPI_CLOCK_DIV8;
      } else if (clock >= SPI_FCLK / 16) {
        clockDiv = SPI_CLOCK_DIV16;
      } else if (clock >= SPI_FCLK / 32) {
        clockDiv = SPI_CLOCK_DIV32;
      } else if (clock >= SPI_FCLK / 64) {
        clockDiv = SPI_CLOCK_DIV64;
      } else {
        clockDiv = SPI_CLOCK_DIV128;
      }
    } else {
      uint32_t clockSetting = SPI_FCLK / 2;
      clockDiv = 0;
      while (clockDiv < 6 && clock < clockSetting) {
        clockSetting /= 2;
        clockDiv++;
      }
    }
   // hspi.Init.BaudRatePrescaler = clockDiv;
    if (bitOrder == MSBFIRST) 
    {
   //     hspi.Init.FirstByte = SPI_LITTLEENDIAN;
    } else {
    //    hspi.Init.FirstByte = SPI_BIGENDIAN;
    }
 }
 // hspi.Init.CLKPolarity = dataMode & 0x10;
 // hspi.Init.CLKPhase = dataMode & 0x01;
  friend class SPIClass;
};


class SPIClass {
public:
  // Initialize the SPI library
  static void begin();

  // If SPI is used from within an interrupt, this function registers
  // that interrupt with the SPI library, so beginTransaction() can
  // prevent conflicts.  The input interruptNumber is the number used
  // with attachInterrupt.  If SPI is used from a different interrupt
  // (eg, a timer), interruptNumber should be 255.
  static void usingInterrupt(uint8_t interruptNumber);
  // And this does the opposite.
  static void notUsingInterrupt(uint8_t interruptNumber);
  // Note: the usingInterrupt and notUsingInterrupt functions should
  // not to be called from ISR context or inside a transaction.
  // For details see:
  // https://github.com/arduino/Arduino/pull/2381
  // https://github.com/arduino/Arduino/pull/2449

  // Before using SPI.transfer() or asserting chip select pins,
  // this function is used to gain exclusive access to the SPI bus
  // and configure the correct settings.
  inline static void beginTransaction(SPISettings settings) {
    begin();
  }

  // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
  inline static uint8_t transfer(uint8_t data) {
    uint8_t recv;
    HAL_SPI_TransmitReceive(&hspi, &data, &recv, 1, 1000);
    return recv;
  }
  inline static uint16_t transfer16(uint16_t data) {
    union
    {
         uint8_t _8recv[2];
         uint16_t recv;
      /* data */
    }in, out;
    in.recv = data;
    HAL_SPI_TransmitReceive(&hspi, in._8recv, out._8recv, 2, 1000);
    return out.recv;
  }
  inline static void transfer(void *buf, size_t count) {
    if (count == 0) return;
       HAL_SPI_Transmit(&hspi, (uint8_t *)buf, count, 1000);
  }
  // After performing a group of transfers and releasing the chip select
  // signal, this function allows others to access the SPI bus
  inline static void endTransaction(void) {
    end();
  }

  // Disable the SPI bus
  static void end();

  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  inline static void setBitOrder(uint8_t bitOrder) {
    if (bitOrder == MSBFIRST) 
    {
        spi_init.FirstByte = SPI_LITTLEENDIAN;
    } else {
        spi_init.FirstByte = SPI_BIGENDIAN;
    }
    HAL_SPI_Init(&hspi);
  }
  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  inline static void setDataMode(uint8_t dataMode) {
        spi_init.CLKPolarity = dataMode & 0x10;
        spi_init.CLKPhase = dataMode & 0x01;
        HAL_SPI_Init(&hspi);

  }
  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  inline static void setClockDivider(uint8_t clockDiv) {
    spi_init.BaudRatePrescaler = clockDiv;
    HAL_SPI_Init(&hspi);
  }
  // These undocumented functions should not be used.  SPI.transfer()
  // polls the hardware flag which is automatically cleared as the
  // AVR responds to SPI's interrupt
 // inline static void attachInterrupt() { }
 // inline static void detachInterrupt() {  }

private:
  static uint8_t initialized;
  static uint8_t interruptMode; // 0=none, 1=mask, 2=global
  static uint8_t interruptMask; // which interrupts to mask
  static uint8_t interruptSave; // temp storage, to restore state
  static SPI_InitTypeDef spi_init; // = {SPI_MODE_MASTER,  SPI_POLARITY_LOW, SPI_PHASE_1EDGE, SPI_NSS_SOFT, SPI_CLOCK_DIV8, SPI_LITTLEENDIAN};
  static SPI_HandleTypeDef hspi;
};
extern SPIClass SPI;

#endif
