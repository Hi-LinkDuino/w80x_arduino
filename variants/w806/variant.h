#ifndef _VARIANT_ARDUINO_W806_
#define _VARIANT_ARDUINO_W806_
#include "pins_arduino.h"
#include "./include/driver/wm_hal.h"
#include "HardwareSerial.h"
#include "WSerial.h"
#define PINS_COUNT          (42U)           /*number of pins on hilink demo board*/
#define ADC_COUNT           (4U)            /*number of adc pins*/
#define PWM_COUNT           (5U)            /*number of pwm pins*/


/**
 * Pin Attributes to be OR-ed
 */
#define PIN_ATTR_NONE          (0UL<<0)
#define PIN_ATTR_COMBO         (1UL<<0)
#define PIN_ATTR_ANALOG        (1UL<<1)
#define PIN_ATTR_DIGITAL       (1UL<<2)
#define PIN_ATTR_PWM           (1UL<<3)
#define PIN_ATTR_TIMER         (1UL<<4)
#define PIN_ATTR_TIMER_ALT     (1UL<<5)
#define PIN_ATTR_EXTINT        (1UL<<6)


typedef enum _EAnalogChannel
{
    No_ADC_Channel  =   -1,
    //ADC_Channel0    =   0,
    ADC_Channel1    =   ADC_ANA_CR_CH_0,
    ADC_Channel2    =   ADC_ANA_CR_CH_1,
    ADC_Channel3    =   ADC_ANA_CR_CH_2,
    ADC_Channel4    =   ADC_ANA_CR_CH_3,
}EAnalogChannel;

// Definitions for PWM channels
typedef enum _EPWMChannel
{
    NOT_ON_PWM      =   -1,
    PWM_CH0        =   0,
    PWM_CH1        =   1,
    PWM_CH2        =   2,
    PWM_CH3        =   3,
    PWM_CH4        =   4,
} EPWMChannel ;

typedef enum _EUARTFunctionIO
{
  NOT_UART_FUNC    = -1,
  UART0_TX_FUNC    = 0,
  UART0_RX_FUNC    = 1,
  UART0_RTS_FUNC   = 2,
  UART0_CTS_FUNC   = 3,       
}EUARTFunctionIO;

/* Types used for the tables below */
typedef struct _PinDescription
{
  GPIO_TypeDef *  pPort ;
  uint32_t  ulPin ;                                  /*pin number*/
  //uint8_t ulPinType ;  
  //uint32_t ulPeripheralId ;
  //uint32_t ulPinConfiguration ;                     
  uint32_t ulPinAttribute ;                         
  EAnalogChannel ulAnalogChannel ;          /* Analog pin in the Arduino context (label on the board) */
  EPWMChannel ulPWMChannel ;              /* PWM pin in the Arduino context (label on the board) */
  //ETCChannel ulTCChannel ;
  EUARTFunctionIO ulUARTFuncIO;           /*UART pin */
} PinDescription ;


extern uint8_t g_pinStatus[];
/* Pins table to be instanciated into variant.cpp */
extern const PinDescription g_APinDescription[] ;


#define ENABLE_HWSERIAL0        1
#define ENABLE_HWSERIAL1        1
#define ENABLE_HWSERIAL2        1
#define ENABLE_HWSERIAL3        1
#define ENABLE_HWSERIAL4        1
#define ENABLE_HWSERIAL5        1

#ifdef __cplusplus
// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR     Serial
#define SERIAL_PORT_HARDWARE    Serial1
#endif

#endif /* _VARIANT_ARDUINO_W806_ */