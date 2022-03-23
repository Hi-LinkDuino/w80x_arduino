
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H

/* Includes ------------------------------------------------------------------*/
// #include "stm32_def.h"
// #include "PinNames.h"
#include "./include/driver/wm_hal.h"
#ifdef __cplusplus
extern "C" {
#endif



// #ifndef UART_IRQ_PRIO                        //TODO use IRQn_Type enmu
// #define UART_IRQ_PRIO       1
// #endif
// #ifndef UART_IRQ_SUBPRIO
// #define UART_IRQ_SUBPRIO    0
// #endif

/* Exported types ------------------------------------------------------------*/
typedef struct serial_s serial_t;

struct serial_s {
  /*  The 1st 2 members USART_TypeDef *uart
   *  and UART_HandleTypeDef handle should
   *  be kept as the first members of this struct
   *  to have get_serial_obj() function work as expected
   */
  USART_TypeDef *uart;                                                //串口结构体
  UART_HandleTypeDef handle;                                          //串口回调句柄
  void (*rx_callback)(serial_t *);                                    //数据中断接收句柄
  int (*tx_callback)(serial_t *);                                     //数据中断发送句柄
  uint8_t pin_tx;                                                     //数据发送引脚
  uint8_t pin_rx;                                                     //数据接收引脚
  uint8_t pin_rts;                                                    //发送请求
  uint8_t pin_cts;                                                    //发送清除
  IRQn_Type irq;                                                      //中断线
  uint32_t priority;                                                  //子优先级
  uint8_t index;                                                      //用来确定串口号 
  uint8_t recv;
  uint8_t *rx_buff;
  uint8_t *tx_buff;                                                    //环形缓冲区指针
  uint16_t rx_tail;                                                    //串口接收读指针
  uint16_t tx_head;                                                     //串口发送写指针
  volatile uint16_t rx_head;                                            //串口接收写指针
  volatile uint16_t tx_tail;                                            //串口发送读指针
  size_t tx_size;                                                      //中断服务函数发送缓存区将要发送数据的长度
};

/* Exported constants --------------------------------------------------------*/
#define TX_TIMEOUT  1000

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void uart_init(serial_t *obj, uint32_t baudrate, uint32_t databits, uint32_t parity, uint32_t stopbits);
void uart_deinit(serial_t *obj);

size_t uart_write(serial_t *obj, uint8_t data, uint16_t size);
int uart_getc(serial_t *obj, unsigned char *c);
void uart_attach_rx_callback(serial_t *obj, void (*callback)(serial_t *));
void uart_attach_tx_callback(serial_t *obj, int (*callback)(serial_t *), size_t size);

uint8_t serial_tx_active(serial_t *obj);
uint8_t serial_rx_active(serial_t *obj);

void uart_enable_tx(serial_t *obj);
void uart_enable_rx(serial_t *obj);

//size_t uart_debug_write(uint8_t *data, uint32_t size);

#endif /* HAL_UART_MODULE_ENABLED  && !HAL_UART_MODULE_ONLY */
#ifdef __cplusplus
}


#endif /* __UART_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
