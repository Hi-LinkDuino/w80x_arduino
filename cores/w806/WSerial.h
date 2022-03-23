#ifndef WIRING_SERIAL_H
#define WIRING_SERIAL_H

#include "variant.h"
#include "HardwareSerial.h"
//#include "wm_uart.h"
#include "./include/driver/wm_hal.h"
//#if defined(HAL_UART_MODULE_ENABLED) && !defined(HAL_UART_MODULE_ONLY)
  //#if !defined(HWSERIAL_NONE) && defined(SERIAL_UART_INSTANCE)          //UART 存在
    
    //TODO SERIAL_UART_INSTANCE 为CUBEMX 实际生成的UART的实例 这里需要替换成W806 
    //#if SERIAL_UART_INSTANCE == 1
  #if !defined(Serial)                      /*定义默认串口为串口0*/
      #define Serial Serial0
      #define serialEvent serialEvent0
  #endif 
 
  
  #if 0
    #if defined (UART0)
      #define ENABLE_HWSERIAL0
      #if !defined(Serial)
        #define Serial Serial1
        #define serialEvent serialEvent1
      #endif
    //#elif SERIAL_UART_INSTANCE == 2
    #elif defined (UART1)
      #define ENABLE_HWSERIAL2
      #if !defined(Serial)
        #define Serial Serial2
        #define serialEvent serialEvent2
      #endif
    //#elif SERIAL_UART_INSTANCE == 3
    #elif UART2
      #define ENABLE_HWSERIAL3
      #if !defined(Serial)
        #define Serial Serial3
        #define serialEvent serialEvent3
      #endif
    //#elif SERIAL_UART_INSTANCE == 4
    #elif UART3
      #define ENABLE_HWSERIAL4
      #if !defined(Serial)
        #define Serial Serial4
        #define serialEvent serialEvent4
      #endif
    //#elif SERIAL_UART_INSTANCE == 5
    #elif UART4
      #define ENABLE_HWSERIAL5
      #if !defined(Serial)
        #define Serial Serial5
        #define serialEvent serialEvent5
      #endif
    //#elif SERIAL_UART_INSTANCE == 6
    #elif UART5
      #define ENABLE_HWSERIAL6
      #if !defined(Serial)
        #define Serial Serial6
        #define serialEvent serialEvent6
      #endif
    #else
      #if !defined(Serial)
        #warning "No generic 'Serial' defined!"
      #endif
    #endif /* SERIAL_UART_INSTANCE == x */
  #endif /* if 0*/
  //#endif /* !HWSERIAL_NONE && SERIAL_UART_INSTANCE */

  #if defined(ENABLE_HWSERIAL0)
    #if defined(UART0_BASE)
      #define HAVE_HWSERIAL0
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL1)
    #if defined(UART1_BASE)
      #define HAVE_HWSERIAL1
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL2)
    #if defined(UART2_BASE)
      #define HAVE_HWSERIAL2
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL3)
    #if defined(UART3_BASE)
      #define HAVE_HWSERIAL3
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL4)
    #if  defined(UART4_BASE)
      #define HAVE_HWSERIAL4
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL5)
    #if defined(UART5_BASE)
      #define HAVE_HWSERIAL5
    #endif
  #endif

  #if defined(HAVE_HWSERIAL0)
    extern void serialEvent0(void) __attribute__((weak));
  #endif
  #if defined(HAVE_HWSERIAL1)
    extern void serialEvent1(void) __attribute__((weak));
  #endif
  #if defined(HAVE_HWSERIAL2)
    extern void serialEvent2(void) __attribute__((weak));
  #endif
  #if defined(HAVE_HWSERIAL3)
    extern void serialEvent3(void) __attribute__((weak));
  #endif
  #if defined(HAVE_HWSERIAL4)
    extern void serialEvent4(void) __attribute__((weak));
  #endif
  #if defined(HAVE_HWSERIAL5)
    extern void serialEvent5(void) __attribute__((weak));
  #endif
//#endif /* HAL_UART_MODULE_ENABLED  && !HAL_UART_MODULE_ONLY */

extern void serialEventRun(void);

#endif /* WIRING_SERIAL_H */
