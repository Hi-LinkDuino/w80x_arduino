#include "variant.h"
#include <stdint.h>

uint8_t g_pinStatus[PINS_COUNT] = {0};

/*
  uint8_t  pPort ;
  uint32_t ulPin ;                                  
  uint32_t ulPinType ;                      
  uint32_t ulPinAttribute ;                         
  EAnalogChannel ulAnalogChannel ;          
*/

const PinDescription g_APinDescription[] = {
    { PIOA, 0,          PIN_ATTR_DIGITAL|PIN_ATTR_ANALOG|PIN_ATTR_PWM,           No_ADC_Channel},
    { PIOA, 1,          PIN_ATTR_DIGITAL|PIN_ATTR_ANALOG|PIN_ATTR_PWM,           ADC_Channel1},
    { PIOA, 2,          PIN_ATTR_DIGITAL|PIN_ATTR_ANALOG|PIN_ATTR_PWM,           ADC_Channel4},
    { PIOA, 3,          PIN_ATTR_DIGITAL|PIN_ATTR_ANALOG|PIN_ATTR_PWM,           ADC_Channel3},
    { PIOA, 4,          PIN_ATTR_DIGITAL|PIN_ATTR_ANALOG|PIN_ATTR_PWM,           ADC_Channel2},
    { PIOA, 5,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOA, 6,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOA, 7,          PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOA, 8,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOA, 9,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOA, 10,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOA, 11,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},       
    { PIOA, 12,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOA, 13,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},    
    { PIOA, 14,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOA, 15,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel},             

    { PIOB, 0,          PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOB, 1,          PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOB, 2,          PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOB, 3,          PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOB, 4,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOB, 5,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOB, 6,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOB, 7,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOB, 8,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOB, 9,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOB, 10,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOB, 11,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel},       
    { PIOB, 12,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOB, 13,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},    
    { PIOB, 14,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOB, 15,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},             
    { PIOB, 16,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOB, 17,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel},       
    { PIOB, 18,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOB, 19,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},    
    { PIOB, 20,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOB, 21,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel}, 
    { PIOB, 22,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel},
    { PIOB, 23,         PIN_ATTR_NONE,                                           No_ADC_Channel},       
    { PIOB, 24,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
    { PIOB, 25,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},    
    { PIOB, 26,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel},
};