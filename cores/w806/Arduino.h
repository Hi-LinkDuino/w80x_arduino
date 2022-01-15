/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "pins_arduino.h"
#include "./include/driver/wm_hal.h"
#include "./include/driver/wm_adc.h"


#ifdef __cplusplus 
extern "C" {
#endif

//Macro-based digital IO fucntions
//#include "wiring_digita.h"

//!!!!#include "binary.h"

// FIXME: workarounds for missing features or unimplemented functions
// cancel out the PROGMEM attribute - used only for atmel CPUs
#define PROGMEM
void yield(void);

// we use pre-defined IRQ function the way wiring does
#define WIRING

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define OUTPUT_OD 0x03
#define INPUT_PULLDOWN 0x4

// undefine mathlib's pi if encountered
#ifdef PI
#undef PI
#endif
#ifdef HALF_PI
#undef HALF_PI
#endif
#ifdef TWO_PI
#undef TWO_PI
#endif

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define FALLING 1

/*
#define INTERNAL1V1 2
#define INTERNAL2V56 3
#define INTERNAL 3
#define DEFAULT 1
#define EXTERNAL 0
*/

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() sei()
#define noInterrupts() cli()

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesPerMillisecond() ( F_CPU / 1000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define byte(w) ((uint8_t)(w))
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define maskSet(value, mask) ((value) |= (mask))
#define maskClear(value, mask) ((value) &= ~(mask))


// avr-libc defines _NOP() since 1.6.2
#ifndef _NOP
//#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif

#define BEGIN_CRITICAL		__critical {
#define END_CRITICAL		}


typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef unsigned char boolean;
typedef unsigned char byte;
//typedef uint8_t byte;

void init(void);
//void initVariant(void);		// weak

//int atexit(void (*func)());	// __attribute__((weak));
//void serialEvent(void);		// weak
//extern unsigned char runSerialEvent;

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);

uint8_t digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
//void analogWrite(uint8_t pin, __xdata uint16_t val);

//uint32_t millis(void);
//uint32_t micros(void);
//void delay(uint32_t ms);
#define millis HAL_GetTick
#define micros HAL_Get_Micros
#define delay(ms) HAL_Delay(ms)

void delayMicroseconds(uint32_t us);
//unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
//unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);

//void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
//uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

//void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), __xdata uint8_t mode);
//void detachInterrupt(uint8_t interruptNum);

void setup(void);
void loop(void);

#ifdef __cplusplus 
}
#endif
#endif


