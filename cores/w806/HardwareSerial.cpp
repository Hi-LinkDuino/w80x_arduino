/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

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

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#include <stdio.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include "WSerial.h"
#include "wm_uart.h"
#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) ||  \
    defined(HAVE_HWSERIAL3) || defined(HAVE_HWSERIAL4) || defined(HAVE_HWSERIAL5) 
  // SerialEvent functions are weak, so when the user doesn't define them,
  // the linker just sets their address to 0 (which is checked below).
  #if defined(HAVE_HWSERIAL0)
    HardwareSerial Serial0(UART0);
    void serialEvent0() __attribute__((weak));            //串口0中断回调函数
  #endif
  
  #if defined(HAVE_HWSERIAL1)
    HardwareSerial Serial1(UART1);
    void serialEvent1() __attribute__((weak));
  #endif

  #if defined(HAVE_HWSERIAL2)
     Serial2(UART2);
    void serialEvent2() __attribute__((weak));
  #endif

  #if defined(HAVE_HWSERIAL3)
    HardwareSerial Serial3(UART3);
    void serialEvent3() __attribute__((weak));
  #endif

  #if defined(HAVE_HWSERIAL4)
    HardwareSerial Serial4(UART4);
    void serialEvent4() __attribute__((weak));
  #endif

  #if defined(HAVE_HWSERIAL5)
    HardwareSerial Serial5(UART5);
    void serialEvent5() __attribute__((weak));
  #endif
#endif // HAVE_HWSERIALx

// Constructors ////////////////////////////////////////////////////////////////
HardwareSerial::HardwareSerial(uint8_t _rx, uint8_t _tx, uint8_t _rts, uint8_t _cts)
{
  init(_rx, _tx, _rts, _cts);
}

/**
 * @brief 初始化函数
 * 
 * @param _rx 引脚
 * @param _tx 
 * @param _rts 
 * @param _cts 
 */
void HardwareSerial::init(uint8_t _rx, uint8_t _tx, uint8_t _rts, uint8_t _cts)
{
  if (_rx == _tx) {
    _serial.pin_rx = 0xff;
  } else {
    _serial.pin_rx = _rx;
  }
  _serial.pin_tx = _tx;
  _serial.pin_rts = _rts;
  _serial.pin_cts = _cts;
  _serial.rx_buff = _rx_buffer;
  _serial.rx_head = 0;
  _serial.rx_tail = 0;
  _serial.tx_buff = _tx_buffer;
  _serial.tx_head = 0;
  _serial.tx_tail = 0;
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////
/**
 * @brief 接收中断回调函数
 * 
 * @param obj 
 */
void HardwareSerial::_rx_complete_irq(serial_t *obj)
{
  // No Parity error, read byte and store it in the buffer if there is room
  unsigned char c;                    

  if (uart_getc(obj, &c) == 0) {          //获取接收的数据

    rx_buffer_index_t i = (unsigned int)(obj->rx_head + 1) % SERIAL_RX_BUFFER_SIZE;     //存放数据的索引

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != obj->rx_tail) {
      obj->rx_buff[obj->rx_head] = c;         //数据存放在头部
      obj->rx_head = i;                       //写入索引
    }
  }
}

// Actual interrupt handlers //////////////////////////////////////////////////
/**
 * @brief 串口发送中断
 * 
 * @param obj 
 * @return int 
 */
int HardwareSerial::_tx_complete_irq(serial_t *obj)
{
  size_t remaining_data;
  // previous HAL transfer is finished, move tail pointer accordingly
  obj->tx_tail = (obj->tx_tail + obj->tx_size) % SERIAL_TX_BUFFER_SIZE;           //将要发送的起始地址=上次发送的起始地址+上次发送的数据长度

  // If buffer is not empty (head != tail), send remaining data
  if (obj->tx_head != obj->tx_tail) {
    remaining_data = (SERIAL_TX_BUFFER_SIZE + obj->tx_head - obj->tx_tail)        //待发送数据长度
                     % SERIAL_TX_BUFFER_SIZE;
    // Limit the next transmission to the buffer end
    // because HAL is not able to manage rollover
    obj->tx_size = min(remaining_data,
                       (size_t)(SERIAL_TX_BUFFER_SIZE - obj->tx_tail));       //最小长度 底层不能完成SERIAL_TX_BUFFER_SIZE 到0地址的跳转 
    uart_attach_tx_callback(obj, _tx_complete_irq, obj->tx_size);             //递归直到数据发送完成
    return -1;
  }

  return 0;
}

// Public Methods //////////////////////////////////////////////////////////////
#define SERIAL_8N1 0x06
#define SERIAL_8N2 0x0E
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E
void HardwareSerial::begin(unsigned long baud, byte config)
{
  uint32_t databits = 0;
  uint32_t stopbits = 0;
  uint32_t parity = 0;

  _baud = baud;
  _config = config;

  // Manage databits              
  switch (config & 0x07) {      // 0000 0000
    case 0x02:                  
      databits = 6;             
      break;                    
    case 0x04:                     
      databits = 7;
      break;
    case 0x06:
      databits = 8;
      break;
    default:
      databits = 0;
      break;
  }

  if ((config & 0x30) == 0x30) {      //奇偶校验
    parity = UART_PARITY_ODD;
    databits++;
  } else if ((config & 0x20) == 0x20) {
    parity = UART_PARITY_EVEN;
    databits++;
  } else {
    parity = UART_PARITY_NONE;
  }

  if ((config & 0x08) == 0x08) {      //停止位
    stopbits = UART_STOPBITS_2;
  } else {
    stopbits = UART_STOPBITS_1;
  }

  switch (databits) {
#ifdef UART_WORDLENGTH_7B
    case 7:
      databits = UART_WORDLENGTH_7B;
      break;
#endif
    case 8:
      databits = UART_WORDLENGTH_8B;
      break;
    case 9:
      //databits = UART_WORDLENGTH_9B;
      break;
    default:
    case 0:
      Error_Handler();
      break;
  }

  uart_init(&_serial, (uint32_t)baud, databits, parity, stopbits);        //串口初始化
  //enableHalfDuplexRx();                                                   //半双工初始化
  uart_attach_rx_callback(&_serial, _rx_complete_irq);                    //注册进HAL的接收中断接口
}

/**
 * @brief 完成发送关闭串口
 * 
 */
void HardwareSerial::end()
{
  // wait for transmission of outgoing data
  flush();        //阻塞数据等待

  uart_deinit(&_serial);      //硬件接口去初始化

  // clear any received data
  _serial.rx_head = _serial.rx_tail;    //清除缓冲区
}

/**
 * @brief 读到的数据个数
 * 
 * @return int 
 */
int HardwareSerial::available(void)       
{
  return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _serial.rx_head - _serial.rx_tail)) % SERIAL_RX_BUFFER_SIZE;
}

/**
 * @brief 返回接收到的下一个字符 不将其从缓冲区清除
 * 
 * @return int 
 */
int HardwareSerial::peek(void)
{
  if (_serial.rx_head == _serial.rx_tail) {       //缓冲区数据为空
    return -1;
  } else {                                        
    return _serial.rx_buff[_serial.rx_tail];
  }
}

/**
 * @brief 读取数据
 * 
 * @return int 
 */
int HardwareSerial::read(void)
{
  //enableHalfDuplexRx();
  // if the head isn't ahead of the tail, we don't have any characters
  if (_serial.rx_head == _serial.rx_tail) {   //缓冲区数据为空
    return -1;
  } else {
    unsigned char c = _serial.rx_buff[_serial.rx_tail];
    _serial.rx_tail = (rx_buffer_index_t)(_serial.rx_tail + 1) % SERIAL_RX_BUFFER_SIZE;   //内存偏移
    return c;
  }
}

/**
 * @brief 获取缓冲区中待写入的数据个数
 * 
 * @return int 
 */
int HardwareSerial::availableForWrite(void)
{
  tx_buffer_index_t head = _serial.tx_head;
  tx_buffer_index_t tail = _serial.tx_tail;

  if (head >= tail) {       //有数据
    return SERIAL_TX_BUFFER_SIZE - 1 - head + tail;
  }   
  return tail - head - 1;
}

/**
 * @brief 等待数据发送完成
 * 
 */
void HardwareSerial::flush()        
{
  // If we have never written a byte, no need to flush. This special
  // case is needed since there is no way to force the TXC (transmit
  // complete) bit to 1 during initialization
  if (!_written) {            //数据可写
    return;
  }

  while ((_serial.tx_head != _serial.tx_tail)) {          //等待发送完成
    // nop, the interrupt handler will free up space for us
  }
  // If we get here, nothing is queued anymore (DRIE is disabled) and
  // the hardware finished transmission (TXC is set).
}

/**
 * @brief 数据写入
 * 
 * @param buffer 数据缓冲区
 * @param size 待写入数据的大小
 * @return size_t 
 */
size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
  size_t size_intermediate;
  size_t ret = size;                        //写入的数据大小
  size_t available = availableForWrite();       //获取带写入的数据大小
  size_t available_till_buffer_end = SERIAL_TX_BUFFER_SIZE - _serial.tx_head;     //单边缓存区的大小

  _written = true;                        //开始数据写入

  // If the output buffer is full, there's nothing for it other than to
  // wait for the interrupt handler to free space
  while (!availableForWrite()) {                                      //等待上次发送完成
    // nop, the interrupt handler will free up space for us
  }

  // HAL doesn't manage rollover, so split transfer till end of TX buffer
  // Also, split transfer according to available space in buffer
  while ((size > available_till_buffer_end) || (size > available)) {    //分割传输 如果传输的大小大于单边缓冲区大小
    size_intermediate = min(available, available_till_buffer_end);      //确定发送大小
    write(buffer, size_intermediate);                                   //数据压入 W（递归其本身啊循环套娃）

    size -= size_intermediate;                                          //从新计算发送长度
    buffer += size_intermediate;                                        //从新计算发送偏移
    available = availableForWrite();                                    //从新计算待写入的数据大小
    available_till_buffer_end = SERIAL_TX_BUFFER_SIZE - _serial.tx_head;
  }

  // Copy data to buffer. Take into account rollover if necessary.
  if (_serial.tx_head + size <= SERIAL_TX_BUFFER_SIZE) {               //需要发送的数据小于实际缓存的大小否则执行切片
    memcpy(&_serial.tx_buff[_serial.tx_head], buffer, size);            //数据拷贝
    size_intermediate = size;                                           //实际发送的数据长度
  } else {
    // memcpy till end of buffer then continue memcpy from beginning of buffer
    size_intermediate = SERIAL_TX_BUFFER_SIZE - _serial.tx_head;        //实际要发送的数据长度
    memcpy(&_serial.tx_buff[_serial.tx_head], buffer, size_intermediate);
    memcpy(&_serial.tx_buff[0], buffer + size_intermediate,           //循环拷贝到tx_buff
           size - size_intermediate);
  }

  // Data are copied to buffer, move head pointer accordingly
  _serial.tx_head = (_serial.tx_head + size) % SERIAL_TX_BUFFER_SIZE; //从新计算tx_head

  // Transfer data with HAL only is there is no TX data transfer ongoing
  // otherwise, data transfer will be done asynchronously from callback
  if (!serial_tx_active(&_serial)) {          //TX空闲
    // note: tx_size correspond to size of HAL data transfer,
    // not the total amount of data in the buffer.
    // To compute size of data in buffer compare head and tail
    _serial.tx_size = size_intermediate;
    uart_attach_tx_callback(&_serial, _tx_complete_irq, size_intermediate);     //发送中断函数
  }

  /* There is no real error management so just return transfer size requested*/
  return ret;
}

/**
 * @brief 写入单个字节的数据
 * 
 * @param c 
 * @return size_t 
 */
size_t HardwareSerial::write(uint8_t c)
{
  uint8_t buff = c;
  return write(&buff, 1);
}

/**
 * @brief 设置数据接收引脚
 * 
 * @param _rx 
 */
void HardwareSerial::setRx(uint8_t _rx)            //引脚配置
{
  _serial.pin_rx = _rx;
}

/**
 * @brief 设置数据发送引脚
 * 
 * @param _tx 
 */
void HardwareSerial::setTx(uint8_t _tx)
{
  _serial.pin_tx = _tx;
}


/**
 * @brief 设置RTS引脚
 * 
 * @param _rts 
 */
void HardwareSerial::setRts(uint8_t _rts)
{
  _serial.pin_rts = _rts;
}

void HardwareSerial::setCts(uint8_t _cts)
{
  _serial.pin_cts = _cts;
}

void HardwareSerial::setRtsCts(uint8_t _rts, uint8_t _cts)
{
  _serial.pin_rts = _rts;
  _serial.pin_cts = _cts;
}
