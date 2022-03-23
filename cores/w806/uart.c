/*
 *******************************************************************************
 * Copyright (c) 2016-2021, STMicroelectronics
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
// #include "core_debug.h"
// #include "lock_resource.h"
#include "uart.h"
#include "Arduino.h"
// #include "PinAF_STM32F1.h"

#ifdef __cplusplus
extern "C" {
#endif
//#if defined(HAL_UART_MODULE_ENABLED) && !defined(HAL_UART_MODULE_ONLY)

/* @brief uart caracteristics */
typedef enum {
#if defined(UART0_BASE)
  UART0_INDEX,
#endif    
#if defined(UART1_BASE)
  UART1_INDEX,
#endif
#if defined(UART2_BASE)
  UART2_INDEX,
#endif
#if defined(UART3_BASE)
  UART3_INDEX,
#endif
#if defined(UART4_BASE)
  UART4_INDEX,
#endif
#if defined(UART5_BASE)
  UART5_INDEX,
#endif
  UART_NUM
} uart_index_t;

static UART_HandleTypeDef *uart_handlers[UART_NUM] = {NULL};            //串口实例

//static serial_t serial_debug = { .uart = NP, .index = UART_NUM };

/* Aim of the function is to get serial_s pointer using huart pointer */
/* Highly inspired from magical linux kernel's "container_of" */
serial_t *get_serial_obj(UART_HandleTypeDef *huart)
{
  struct serial_s *obj_s;
  serial_t *obj;

  obj_s = (struct serial_s *)((char *)huart - offsetof(struct serial_s, handle));
  obj = (serial_t *)((char *)obj_s - offsetof(serial_t, uart));

  return (obj);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)            //UART_HandleTypeDef 无法传递GPIO进入 
                                                            //这里直接在uart_init 完成IO初始化
{
    ;;;
}


/**
  * @brief  Function called to initialize the uart interface
  * @param  obj : pointer to serial_t structure
  * @retval None
  */
void uart_init(serial_t *obj, uint32_t baudrate, uint32_t databits, uint32_t parity, uint32_t stopbits)
{
  if (obj == NULL) {
    return;
  }

  UART_HandleTypeDef *huart = &(obj->handle);
    /*校验保证每个IO都具有UART功能*/

    uint32_t uart_tx_ret = g_APinDescription[obj->pin_tx].ulUARTFuncIO;
    uint32_t uart_rx_ret = g_APinDescription[obj->pin_rx].ulUARTFuncIO;
    uint32_t uart_rts_ret = g_APinDescription[obj->pin_rts].ulUARTFuncIO;
    uint32_t uart_cts_ret = g_APinDescription[obj->pin_cts].ulUARTFuncIO;

    if((obj->pin_tx == 0xff) || uart_tx_ret == NOT_UART_FUNC)     //uncheck
    {
        return ;
    }
    if((obj->pin_rx == 0xff) || uart_rx_ret == NOT_UART_FUNC)     //uncheck
    {
        return ;
    } 
    if((obj->pin_rts != 0xff) || uart_rts_ret == NOT_UART_FUNC)   
    {
        return ;
    }  
    if((obj->pin_cts != 0xff) || uart_cts_ret == NOT_UART_FUNC)   
    {
        return ;
    }   
    /*转换 pin to UartHandleDef*/
    if((uart_tx_ret == UART0_TX_FUNC) || (uart_rx_ret == UART0_RX_FUNC))            //TODO 此处不够健壮
        obj->uart = (USART_TypeDef *)UART0;
#if 0        
    else if(uart_tx_ret == UART1_TX_FUNC) || (uart_rx_ret == UART1_RX_FUNC)
        obj->uart = (USART_TypeDef *)UART1;        
    else if(uart_tx_ret == UART2_TX_FUNC) || (uart_rx_ret == UART2_RX_FUNC)
        obj->uart = (USART_TypeDef *)UART2;  
#endif         
  /* Enable UART clock */
#if defined(UART0_BASE)
  else if (obj->uart == UART0) {
    __HAL_RCC_UART0_CLK_ENABLE();
    obj->index = UART0_INDEX;
    obj->irq = UART0_IRQn;
    obj->priority = 0;
  }
#endif    
#if defined(UART1_BASE)
  else if (obj->uart == UART1) {
    __HAL_RCC_UART1_CLK_ENABLE();
    obj->index = UART1_INDEX;
    obj->irq = UART1_IRQn;
    obj->priority = 1;
  }
#endif  
#if defined(UART2_BASE)
  else if (obj->uart == UART2) {
    __HAL_RCC_UART2_CLK_ENABLE();
    obj->index = UART2_INDEX;
    obj->irq = UART2_5_IRQn;
    obj->priority = 2;
  }
#endif
#if defined(UART3_BASE)
  else if (obj->uart == UART3) {
    __HAL_RCC_UART2_CLK_ENABLE();
    obj->index = UART3_INDEX;
    obj->irq = UART2_5_IRQn;
  }
#endif
#if defined(UART4_BASE)
  else if (obj->uart == UART4) {
    __HAL_RCC_UART4_CLK_ENABLE();
    obj->index = UART4_INDEX;
    obj->irq = UART2_5_IRQn;
    obj->priority = 2;
  }
#endif
#if defined(UART5_BASE)
  else if (obj->uart == UART5) {
    __HAL_RCC_UART5_CLK_ENABLE();
    // __HAL_RCC_UART5_FORCE_RESET();
    // __HAL_RCC_UART5_RELEASE_RESET();
    // __HAL_RCC_UART5_CLK_ENABLE();
    obj->index = UART5_INDEX;
    obj->irq = UART2_5_IRQn;
    obj->priority = 2;
  }
#endif

  /* Configure UART GPIO pins */
  //g_APinDescription[obj->pin_tx].pPort g_APinDescription[obj->pin_tx].ulPin
  if(uart_tx_ret == UART0_TX_FUNC)
  {
    __HAL_AFIO_REMAP_UART0_TX(g_APinDescription[obj->pin_tx].pPort, g_APinDescription[obj->pin_tx].ulPin);   
  }
  if(uart_rx_ret == UART0_RX_FUNC)
  {
    __HAL_AFIO_REMAP_UART0_RX(g_APinDescription[obj->pin_rx].pPort, g_APinDescription[obj->pin_rx].ulPin);
  }
    
  /* Configure flow control */
  uint32_t flow_control = UART_HWCONTROL_NONE;

  if(uart_rts_ret == UART0_RTS_FUNC)                        //配置RTS引脚
  {
      __HAL_AFIO_REMAP_UART0_RTS(g_APinDescription[obj->pin_rts].pPort, g_APinDescription[obj->pin_rts].ulPin);
  }  
  if(uart_cts_ret == UART0_CTS_FUNC)                        //配置RTS引脚
  {
      __HAL_AFIO_REMAP_UART0_RTS(g_APinDescription[obj->pin_cts].pPort, g_APinDescription[obj->pin_cts].ulPin);
  }  
    //TODO 需要填写除了串口0 以外的串口初始化
    
  /* Configure uart */
  uart_handlers[obj->index] = huart;
  huart->Instance          = (USART_TypeDef *)(obj->uart);
  huart->Init.BaudRate     = baudrate;
  huart->Init.WordLength   = databits;
  huart->Init.StopBits     = stopbits;
  huart->Init.Parity       = parity;
  huart->Init.Mode         = UART_MODE_TX_RX;
  huart->Init.HwFlowCtl    = flow_control;
  
  //huart->Init.OverSampling = UART_OVERSAMPLING_16;            //TODO this them needn`t set
  //huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;       //TODO STM32 need it
  //huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;       //#ifdef UART_ONE_BIT_SAMPLE_DISABLE

  /* Set the NVIC priority for future interrupts */
    HAL_NVIC_SetPriority(obj->irq, obj->priority);
    HAL_NVIC_EnableIRQ(obj->irq);

  if (HAL_UART_Init(huart) != HAL_OK) {             //TODO no helf uart
    return;
  }
}

/**
  * @brief  Function called to deinitialize the uart interface
  * @param  obj : pointer to serial_t structure
  * @retval None
  */
void uart_deinit(serial_t *obj)
{
  /* Reset UART and disable clock */
  switch (obj->index) {
#if defined(UART0_BASE)
    case UART0_INDEX:
        __HAL_RCC_UART0_CLK_DISABLE();
    //   __HAL_RCC_USART1_FORCE_RESET();
    //   __HAL_RCC_USART1_RELEASE_RESET();
    //   __HAL_RCC_USART1_CLK_DISABLE();
      break;
#endif      
#if defined(UART1_BASE)
    case UART1_INDEX:
        __HAL_RCC_UART1_CLK_DISABLE();
    //   __HAL_RCC_USART1_FORCE_RESET();
    //   __HAL_RCC_USART1_RELEASE_RESET();
    //   __HAL_RCC_USART1_CLK_DISABLE();
      break;
#endif
#if defined(UART2_BASE)
    case UART2_INDEX:
        __HAL_RCC_UART2_CLK_DISABLE();
    //   __HAL_RCC_USART2_FORCE_RESET();
    //   __HAL_RCC_USART2_RELEASE_RESET();
    //   __HAL_RCC_USART2_CLK_DISABLE();
      break;
#endif
#if defined(UART3_BASE)
    case UART3_INDEX:
        __HAL_RCC_UART3_CLK_DISABLE();
    //   __HAL_RCC_USART3_FORCE_RESET();
    //   __HAL_RCC_USART3_RELEASE_RESET();
    //   __HAL_RCC_USART3_CLK_DISABLE();
      break;
#endif
#if defined(UART4_BASE)
    case UART4_INDEX:
        __HAL_RCC_UART4_CLK_DISABLE();
    //   __HAL_RCC_UART4_FORCE_RESET();
    //   __HAL_RCC_UART4_RELEASE_RESET();
    //   __HAL_RCC_UART4_CLK_DISABLE();
      break;
#endif
#if defined(UART5_BASE)
    case UART5_INDEX:
        __HAL_RCC_UART1_CLK_DISABLE();
    //   __HAL_RCC_UART5_FORCE_RESET();
    //   __HAL_RCC_UART5_RELEASE_RESET();
    //   __HAL_RCC_UART5_CLK_DISABLE();
      break;
#endif
  }
  HAL_UART_DeInit(uart_handlers[obj->index]);
}


/**
  * @brief  write the data on the uart
  * @param  obj : pointer to serial_t structure
  * @param  data : byte to write
  * @param  size : number of data to write
  * @retval The number of bytes written
  */
size_t uart_write(serial_t *obj, uint8_t data, uint16_t size)
{
  if (HAL_UART_Transmit(uart_handlers[obj->index], &data, size, TX_TIMEOUT) == HAL_OK) {
    return size;
  } else {
    return 0;
  }
}


/**
 * Attempts to determine if the serial peripheral is already in use for RX
 *
 * @param obj The serial object
 * @return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_rx_active(serial_t *obj)
{
  //return ((HAL_UART_GetState(uart_handlers[obj->index]) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX);
    return (uart_handlers[obj->index]->RxState ==  HAL_UART_STATE_BUSY_RX);
}

/**
 * Attempts to determine if the serial peripheral is already in use for TX
 *
 * @param obj The serial object
 * @return Non-zero if the TX transaction is ongoing, 0 otherwise
 */
uint8_t serial_tx_active(serial_t *obj)
{
  //return ((HAL_UART_GetState(uart_handlers[obj->index]) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX);
  return (uart_handlers[obj->index]->gState == HAL_UART_STATE_BUSY_TX);
}

/**
  * @brief  Read receive byte from uart
  * @param  obj : pointer to serial_t structure
  * @retval last character received
  */
int uart_getc(serial_t *obj, unsigned char *c)
{
  if (obj == NULL) {
    return -1;
  }

  if (serial_rx_active(obj)) {                  //RX接收是否忙碌
    return -1; /* Transaction ongoing */
  }

  *c = (unsigned char)(obj->recv);              //拿到接收数据
  /* Restart RX irq */                          //从新启动中断接口
  HAL_UART_Receive_IT(uart_handlers[obj->index], &(obj->recv), 1);

  return 0;
}

/**
 * Begin asynchronous RX transfer (enable interrupt for data collecting)
 *
 * @param obj : pointer to serial_t structure
 * @param callback : function call at the end of reception
 * @retval none
 */
void uart_attach_rx_callback(serial_t *obj, void (*callback)(serial_t *))
{
  if (obj == NULL) {
    return;
  }

  /* Exit if a reception is already on-going */
  if (serial_rx_active(obj)) {
    return;
  }
  obj->rx_callback = callback;

  /* Must disable interrupt to prevent handle lock contention */
  HAL_NVIC_DisableIRQ(obj->irq);

  HAL_UART_Receive_IT(uart_handlers[obj->index], &(obj->recv), 1);

  /* Enable interrupt */
  HAL_NVIC_EnableIRQ(obj->irq);
}

/**
 * Begin asynchronous TX transfer.
 *
 * @param obj : pointer to serial_t structure
 * @param callback : function call at the end of transmission
 * @retval none
 */
void uart_attach_tx_callback(serial_t *obj, int (*callback)(serial_t *), size_t size)
{
  if (obj == NULL) {
    return;
  }
  obj->tx_callback = callback;

  /* Must disable interrupt to prevent handle lock contention */
  HAL_NVIC_DisableIRQ(obj->irq);

  /* The following function will enable UART_IT_TXE and error interrupts */
  HAL_UART_Transmit_IT(uart_handlers[obj->index], &obj->tx_buff[obj->tx_tail], size);

  /* Enable interrupt */
  HAL_NVIC_EnableIRQ(obj->irq);
}

/**
  * @brief  Return index of the serial handler
  * @param  UartHandle pointer on the uart reference
  * @retval index
  */
/*                              
uint8_t uart_index(UART_HandleTypeDef *huart)               //STM32 Duino is not define this func 
{
  uint8_t i = 0;
  if (huart == NULL) {
    return UART_NUM;
  }

  for (i = 0; i < UART_NUM; i++) {
    if (huart == uart_handlers[i]) {
      break;
    }
  }

  return i;
}
*/

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle pointer on the uart reference
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  serial_t *obj = get_serial_obj(huart);
  if (obj) {
    obj->rx_callback(obj);
  }
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle pointer on the uart reference
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  serial_t *obj = get_serial_obj(huart);
  if (obj) {
    obj->tx_callback(obj);
  }
}

#if 0
/**
  * @brief  error callback from UART
  * @param  UartHandle pointer on the uart reference
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
#if defined(STM32F1xx) || defined(STM32F2xx) || defined(STM32F4xx) || defined(STM32L1xx)
  if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) != RESET) {
    __HAL_UART_CLEAR_PEFLAG(huart); /* Clear PE flag */
  } else if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) != RESET) {
    __HAL_UART_CLEAR_FEFLAG(huart); /* Clear FE flag */
  } else if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE) != RESET) {
    __HAL_UART_CLEAR_NEFLAG(huart); /* Clear NE flag */
  } else if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
    __HAL_UART_CLEAR_OREFLAG(huart); /* Clear ORE flag */
  }
#else
  if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) != RESET) {
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_PEF); /* Clear PE flag */
  } else if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) != RESET) {
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_FEF); /* Clear FE flag */
  } else if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE) != RESET) {
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_NEF); /* Clear NE flag */
  } else if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF); /* Clear ORE flag */
  }
#endif
  /* Restart receive interrupt after any error */
  serial_t *obj = get_serial_obj(huart);
  if (obj && !serial_rx_active(obj)) {
    HAL_UART_Receive_IT(huart, &(obj->recv), 1);
  }
}
#endif

#if defined(UART0_BASE)
void UART0_IRQHandler(void)
{
    HAL_UART_IRQHandler(uart_handlers[UART0_INDEX]);
}
#endif

#if defined(UART1_BASE)
void UART1_IRQHandler(void)                     //TODO ！！！！一定要保证程序链接进去了！！！！！！ 
{
//   HAL_NVIC_ClearPendingIRQ(USART1_IRQn);
    HAL_UART_IRQHandler(uart_handlers[UART1_INDEX]);
}
#endif

/**
  * @brief  UART 2 IRQ handler
  * @param  None
  * @retval None
  */
#if (defined(UART2_BASE) || defined(UART3_BASE) || defined(UART4_BASE) || defined(UART5_BASE))
void UART2_5_IRQHandler(void)
{
  //HAL_NVIC_ClearPendingIRQ(USART2_IRQn);
    if (uart_handlers[UART2_INDEX] != NULL) {
    HAL_UART_IRQHandler(uart_handlers[UART2_INDEX]);
    }
    if (uart_handlers[UART3_INDEX] != NULL) {
    HAL_UART_IRQHandler(uart_handlers[UART3_INDEX]);
    }
    if (uart_handlers[UART4_INDEX] != NULL) {
    HAL_UART_IRQHandler(uart_handlers[UART4_INDEX]);
    }
    if (uart_handlers[UART5_INDEX] != NULL) {
    HAL_UART_IRQHandler(uart_handlers[UART5_INDEX]);
    }            
}
#endif


#if 0
/**
  * @brief  HAL UART Call Back
  * @param  UART handler
  * @retval None
  */
void HAL_UARTEx_WakeupCallback(UART_HandleTypeDef *huart)
{
  serial_t *obj = get_serial_obj(huart);
  HAL_UART_Receive_IT(huart,  &(obj->recv), 1);
}
#endif

#ifdef __cplusplus
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
