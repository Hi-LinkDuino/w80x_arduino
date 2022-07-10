

#if 1

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
sssssssssss
/*HSPI default port & pin define*/
#define SPI_CS_DEF_PIN (PB4)
#define SPI_CLK_DEF_PIN (PB2)
#define SPI_MISO_DEF_PIN (PB3)
#define SPI_MOSI_DEF_PIN (PB5)
// SPIClass((SPI_TypeDef *)spi_base = SPI);
// SPIClass::SPIClass(SPI_TypeDef *spi_base)
SPIClass::SPIClass()
#if 1
    :
      //_spi(NULL)
      // _use_hw_ss(false),
      _sck(-1), _miso(-1), _mosi(-1), _ss(-1)
      // ,_div(0)
      ,_spi_id(HSPI_BASE)
      ,_freq(SPI_BAUDRATEPRESCALER_40), _inTransaction(false)
#endif
//, _ss_mode(-1), _data_mode(-1), _bit_order(-1)
{
  printf("--->[%s][%s] set instance val\r\n", __FUNCTION__, __FILE__);
  // _sck = -1;
  // _miso = -1;
  // _mosi = -1;
  // _ss = -1;
  // _freq = SPI_BAUDRATEPRESCALER_40;
  // _inTransaction = false;

  //  _spi.Instance = HSPI_BASE;

  // // if(spi_bus == 0)
  // {
  //   printf("---> set instance val\r\n");
  //    _spi.Instance = HSPI_BASE;
  // }
 
}

SPIClass::~SPIClass()
{
  end();
}

void SPIClass::begin(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
{
    _spi.Instance = _spi_id;//HSPI_BASE;
    printf("_spi_id[%x]\r\n", _spi_id);
#if 1  
printf("spi begin func\r\n");
  GPIO_TypeDef *port_cs, *port_clk, *port_miso, *port_mosi;
    // GPIO_TypeDef *port_cs, *port_clk, *port_miso, *port_mosi;
  uint32_t gpio_cs = 0;
  uint32_t gpio_clk = 0;
  uint32_t gpio_miso = 0;
  uint32_t gpio_mosi = 0;

  printf("base addr [%d]\r\n", _spi.Instance);
  if (_spi.Instance == NULL)
  {
    printf(" spi_instance is not init\r\n");
    return;
  }

#if 0
    if(!_div) {       /*计算分频系数*/
        // _div = spiFrequencyToClockDiv(_freq);
    }
#endif

  if (sck == -1 && miso == -1 && mosi == -1 && ss == -1)
  { /*默认配置*/
    printf("默认配置\r\n");
    if (_spi.Instance == HSPI_BASE)
    {
      _sck = SPI_CLK_DEF_PIN;
      _miso = SPI_MISO_DEF_PIN;
      _mosi = SPI_MOSI_DEF_PIN;
      _ss = SPI_CS_DEF_PIN;
    }
  }
  else
  { /*复用引脚配置*/
   printf("复用引脚配置\r\n");
    _sck = sck;
    _miso = miso;
    _mosi = mosi;
    _ss = ss;
  }
  /*init GPIO*/
  __HAL_RCC_SPI_CLK_ENABLE(); /*Enable SPI clock*/
//  GPIOB_BASE
  arduino_pin_2_gpio(_ss, &port_cs, &gpio_cs);
  arduino_pin_2_gpio(_sck, &port_clk, &gpio_clk);
  arduino_pin_2_gpio(_miso, &port_miso, &gpio_miso);
  arduino_pin_2_gpio(_mosi, &port_mosi, &gpio_mosi);

  printf("1init port-->[%x]\r\n", port_cs);

  // printf("3init port-->[%x]\r\n", *port_cs->DATA);


  printf("cs_pin[%d]\r\n", gpio_cs);
  printf("clk_pin[%d]\r\n", gpio_clk);
  printf("miso_pin[%d]\r\n", gpio_miso);
  printf("mosi_pin[%d]\r\n", gpio_mosi);  

  __HAL_AFIO_REMAP_SPI_CS(port_cs, gpio_cs);
  __HAL_AFIO_REMAP_SPI_CLK(port_clk, gpio_clk);
  __HAL_AFIO_REMAP_SPI_MISO(port_miso, gpio_miso);
  __HAL_AFIO_REMAP_SPI_MOSI(port_mosi, gpio_mosi);

	// __HAL_AFIO_REMAP_SPI_CS(GPIOB, GPIO_PIN_4);
	// __HAL_AFIO_REMAP_SPI_CLK(GPIOB, GPIO_PIN_2);
	// __HAL_AFIO_REMAP_SPI_MISO(GPIOB, GPIO_PIN_3);
	// __HAL_AFIO_REMAP_SPI_MOSI(GPIOB, GPIO_PIN_5);
	

  /*SPI 外设初始化*/
  // _spi.Instance = SPI;             //begin 的时候直接传递进来
  _spi.Init.Mode = SPI_MODE_MASTER; //主机模式

  // SPIModeSet(&_spi, SPI_MODE3);     //TODO 直接通过结构体进行设置
  // setBitOrder(SPI_LSBFIRST);        //BigEndding

  SPIModeSet(&_spi, SPI_MODE0);     //TODO 直接通过结构体进行设置
  setBitOrder(SPI_MSBFIRST);        //BigEndding
  //TODO 这里直接判断结构体，在初始化的时候对结构体进行赋值
  // _spi.Init.NSS = (_ss_mode == -1) ? SPI_NSS_SOFT : _ss_mode;
  // _spi.Init.BaudRatePrescaler = (_freq == SPI_BAUDRATEPRESCALER_40) ? SPI_BAUDRATEPRESCALER_40 : _freq;
  // _spi.Init.FirstByte = (_bit_order == -1) ? SPI_LITTLEENDIAN : _bit_order;

  if (HAL_SPI_Init(&_spi) != HAL_OK)
  {
    Error_Handler();
  }
#endif  
}


void SPIClass::end()
{
#if 1  
  GPIO_TypeDef *port_cs, *port_clk, *port_miso, *port_mosi;
  uint32_t gpio_cs, gpio_clk, gpio_miso, gpio_mosi;

  if (!_spi.Instance)
  {
    return;
  }
  // test_func(2);


  //TODO临时去掉
   HAL_SPI_DeInit(&_spi);
  __HAL_RCC_SPI_CLK_DISABLE();


  //TODO临时去掉
  arduino_pin_2_gpio(_ss, &port_cs, &gpio_cs);
  arduino_pin_2_gpio(_sck, &port_clk, &gpio_clk);
  arduino_pin_2_gpio(_miso, &port_miso, &gpio_miso);
  arduino_pin_2_gpio(_mosi, &port_mosi, &gpio_mosi);


  HAL_GPIO_DeInit(port_cs, gpio_cs);
  HAL_GPIO_DeInit(port_clk, gpio_clk);
  HAL_GPIO_DeInit(port_miso, gpio_miso);
  HAL_GPIO_DeInit(port_mosi, gpio_mosi);
 
 
  //IO 去初始化


  //SPI 去初始化

  _spi.Instance = NULL;
#endif  
}

#if 1
void SPIClass::setHwCs(bool use)
{
  if (!_spi.Instance)
  {
    return;
  }

  if (use == true)
    _spi.Init.NSS = SPI_NSS_HARD;
  else
    _spi.Init.NSS = SPI_NSS_SOFT;
  // _use_hw_ss = use;
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
  uint8_t current_order = 0;
  current_order = _spi.Init.FirstByte;

  if (current_order != bitOrder)
  {
    _spi.Init.FirstByte = bitOrder;
    // if (HAL_SPI_Init(&_spi) != HAL_OK)
    // {
    //   Error_Handler();
    // }
  }
  // spiSetBitOrder(_spi, bitOrder);
}

void SPIClass::setDataMode(uint8_t dataMode)
{
  uint8_t current_mode = 0;
  current_mode = SPIModeGet(&_spi);

  if (current_mode != dataMode)
  {
    SPIModeSet(&_spi, dataMode);
    // if (HAL_SPI_Init(&_spi) != HAL_OK)
    // {
    //   Error_Handler();
    // }
  }
  // spiSetDataMode(_spi, dataMode);
}


void SPIClass::setFrequency(uint32_t freq)
{
  uint32_t current_freq = _spi.Init.BaudRatePrescaler;
  if (current_freq != freq)
  {
    _spi.Init.BaudRatePrescaler = freq;
    // if (HAL_SPI_Init(&_spi) != HAL_OK)
    // {
    //   Error_Handler();
    // }
  }
}
#endif

#if 1





void SPIClass::beginTransaction(SPISettings settings)
{
  setFrequency(settings._clock);
  setBitOrder(settings._bitOrder);
  SPIModeSet(&_spi, settings._dataMode);

  if (HAL_SPI_Init(&_spi) != HAL_OK)
  {
    Error_Handler();
  }
  // HAL_SPI_TransmitReceive(&_spi, (uint8_t *)tx_data, (uint8_t *)rx_data, data_len, 0xFFFF);
  // SPI_PARAM_LOCK();

  //check if last freq changed
#if 0    
    uint32_t cdiv = spiGetClockDiv(_spi);
    if(_freq != settings._clock || _div != cdiv) {
        _freq = settings._clock;
        _div = spiFrequencyToClockDiv(_freq);
    }
  
    spiTransaction(_spi, _div, settings._dataMode, settings._bitOrder);
#endif
  _inTransaction = true;
}

void SPIClass::endTransaction()
{
  if (_inTransaction)
  {
    _inTransaction = false;

    HAL_SPI_DeInit(&_spi); // <-- Im not sure should it be here or right after spiTransaction()
    // spiEndTransaction(_spi);
    // SPI_PARAM_UNLOCK(); // <-- Im not sure should it be here or right after spiTransaction()
  }
}

void SPIClass::write(uint8_t data) //仅仅写入数据
{
  if (_inTransaction)
  {
    // return spiWriteByteNL(_spi, data);     //等待发送完成
  }

  HAL_SPI_Transmit(&_spi, &data, 1, SPI_TIME_OUT_DEF);
  // spiWriteByte(_spi, data);
}

uint8_t SPIClass::transfer(uint8_t data) //双向数据发送
{
  uint8_t temp_out = 0;
  if (_inTransaction)
  {
    // return spiTransferByteNL(_spi, data);  //等待发送完成
  }
  // return spiTransferByte(_spi, data);

  HAL_SPI_TransmitReceive(&_spi, &data, &temp_out, 1, SPI_TIME_OUT_DEF);

  return (temp_out);
}

void SPIClass::write16(uint16_t data)
{
  uint8_t temp_in[2]  = {0};
  
  
  temp_in[0] = highByte(data);
  temp_in[1] = lowByte(data);

  if (_inTransaction)
  {
    // return spiWriteShortNL(_spi, data);
  }
  // spiWriteWord(_spi, data);
  HAL_SPI_Transmit(&_spi, temp_in, 2, SPI_TIME_OUT_DEF);
}


uint16_t SPIClass::transfer16(uint16_t data)
{
  uint8_t temp_in[2]  = {0};
  uint8_t temp_out[2] = {0};
  uint16_t temp_data = 0;
  if (_inTransaction)
  {
    // return spiTransferShortNL(_spi, data);
  }


  temp_in[0] = highByte(data);
  temp_in[1] = lowByte(data);

  HAL_SPI_TransmitReceive(&_spi, temp_in, temp_out, 2, SPI_TIME_OUT_DEF);

  temp_data = temp_out[0];
  temp_data = (temp_data<<8)|temp_out[1];

  return temp_data;
  // return spiTransferWord(_spi, data);
}

void SPIClass::write32(uint32_t data)
{
  uint8_t temp_in[4]  = {0};
  
  temp_in[0] = data>>24;
  temp_in[1] = (data>>16) & 0xFF;
  temp_in[2] = (data>>8) & 0xFF;
  temp_in[3] = data & 0xFF;

  HAL_SPI_Transmit(&_spi, temp_in, 4, SPI_TIME_OUT_DEF);
  if (_inTransaction)
  {
    // return spiWriteLongNL(_spi, data);
  }
  // spiWriteLong(_spi, data);
}

uint32_t SPIClass::transfer32(uint32_t data)
{
  uint8_t temp_in[4]  = {0};
  uint8_t temp_out[4] = {0};
  uint32_t temp_data = 0;

  if (_inTransaction)
  {
    // return spiTransferLongNL(_spi, data);
  }

  temp_in[0] = data>>24;
  temp_in[1] = (data>>16) & 0xFF;
  temp_in[2] = (data>>8) & 0xFF;
  temp_in[3] = data & 0xFF;

  HAL_SPI_TransmitReceive(&_spi, temp_in, temp_out, 4, SPI_TIME_OUT_DEF);

  temp_data = temp_out[0];
  temp_data = (temp_data<<8) | temp_out[1];
  temp_data = (temp_data<<8) | temp_out[2];
  temp_data = (temp_data<<8) | temp_out[3];

  return temp_data;
  // return spiTransferLong(_spi, data);
}

#if 0
void SPIClass::transferBits(uint32_t data, uint32_t *out, uint8_t bits) //考虑是不是要实现
{
  if (_inTransaction)
  {
    // return spiTransferBitsNL(_spi, data, out, bits);
  }
  // spiTransferBits(_spi, data, out, bits);
}
#endif

void SPIClass::writeBytes(uint8_t *data, uint32_t size) //多字节写入
{
  if (_inTransaction)
  {
    // return spiWriteNL(_spi, data, size);
  }

   HAL_SPI_Transmit(&_spi, data, size, SPI_TIME_OUT_DEF);
  // spiSimpleTransaction(_spi);
  // spiWriteNL(_spi, data, size);
  // spiEndTransaction(_spi);
}

void SPIClass::transfer(uint8_t *data, uint32_t size)
{
  // transferBytes((const uint8_t *)data, (uint8_t *)data, size);
   HAL_SPI_Transmit(&_spi, data, size, SPI_TIME_OUT_DEF);
}

/**
 * @param data uint8_t * data buffer. can be NULL for Read Only operation
 * @param out  uint8_t * output buffer. can be NULL for Write Only operation
 * @param size uint32_t
 */
void SPIClass::transferBytes(uint8_t *data, uint8_t *out, uint32_t size)
{
  //需要处理NULL的情况
  if (_inTransaction)
  {
    // return spiTransferBytesNL(_spi, data, out, size);
  }
  HAL_SPI_TransmitReceive(&_spi, data, out, size, SPI_TIME_OUT_DEF);
  // spiTransferBytes(_spi, data, out, size);
}

#if 0
/**
 * @param data uint8_t *
 * @param size uint8_t  max for size is 64Byte
 * @param repeat uint32_t
 */
void SPIClass::writePattern(const uint8_t *data, uint8_t size, uint32_t repeat)
{
  if (size > 64)
  {
    return; //max Hardware FIFO
  }

  uint32_t byte = (size * repeat);
  uint8_t r = (64 / size);
  const uint8_t max_bytes_FIFO = r * size; // Max number of whole patterns (in bytes) that can fit into the hardware FIFO

  while (byte)
  {
    if (byte > max_bytes_FIFO)
    {
      writePattern_(data, size, r);
      byte -= max_bytes_FIFO;
    }
    else
    {
      writePattern_(data, size, (byte / size));
      byte = 0;
    }
  }
}

void SPIClass::writePattern_(const uint8_t *data, uint8_t size, uint8_t repeat)
{
  uint8_t bytes = (size * repeat);
  uint8_t buffer[64];
  uint8_t *bufferPtr = &buffer[0];
  const uint8_t *dataPtr;
  uint8_t dataSize = bytes;
  for (uint8_t i = 0; i < repeat; i++)
  {
    dataSize = size;
    dataPtr = data;
    while (dataSize--)
    {
      *bufferPtr = *dataPtr;
      dataPtr++;
      bufferPtr++;
    }
  }

  writeBytes(&buffer[0], bytes);
}
#endif

#endif

void SPIClass::Error_Handler(void)
{
  while (1)
  {
  }
}

//后面修改名称为setSpiMode( uint8_t mode)
void SPIClass::SPIModeSet(SPI_HandleTypeDef *_spi, uint8_t mode)
{
  /*1: spi clk line is high when idle     0: spi clk line is low when idle*/
  /*the bit combined with spi_mosi_delay_mode bits to set mosi signal delay mode.*/
  if (!_spi)
  {
    return;
  }

  if (SPIModeGet(_spi) == mode) //mode is not change
  {
    return;
  }

  switch (mode)
  {
  case (SPI_MODE0):
    /*
  CPOL = 0：空闲时是低电平，第1个跳变沿是上升沿，第2个跳变沿是下降沿
  CPHA = 0：数据在第1个跳变沿（上升沿）采样
  */
    _spi->Init.CLKPolarity = SPI_POLARITY_LOW;
    _spi->Init.CLKPhase = SPI_PHASE_1EDGE;
    break;

  case (SPI_MODE1):
    /*
  CPOL = 0：空闲时是低电平，第1个跳变沿是上升沿，第2个跳变沿是下降沿
  CPHA = 1：数据在第2个跳变沿（下降沿）采样
  */
    _spi->Init.CLKPolarity = SPI_POLARITY_LOW;
    _spi->Init.CLKPhase = SPI_PHASE_2EDGE;
    break;

  case (SPI_MODE2):
    /*
  CPOL = 1：空闲时是高电平，第1个跳变沿是下降沿，第2个跳变沿是上升沿
  CPHA = 0：数据在第1个跳变沿（下降沿）采样
  */
    _spi->Init.CLKPolarity = SPI_POLARITY_HIGH;
    _spi->Init.CLKPhase = SPI_PHASE_1EDGE;
    break;
  case (SPI_MODE3):
  default:
    /*
  CPOL = 1：空闲时是高电平，第1个跳变沿是下降沿，第2个跳变沿是上升沿
  CPHA = 1：数据在第2个跳变沿（上升沿）采样
  */
    _spi->Init.CLKPolarity = SPI_POLARITY_HIGH;
    _spi->Init.CLKPhase = SPI_PHASE_2EDGE;
    break;
  }
}

uint8_t SPIClass::SPIModeGet(SPI_HandleTypeDef *_spi)
{
  uint8_t mode = SPI_MODE3;

  if (_spi->Init.CLKPolarity == SPI_POLARITY_LOW)
  {
    if (_spi->Init.CLKPhase == SPI_PHASE_1EDGE)
    {
      mode = SPI_MODE0;
    }
    else if (_spi->Init.CLKPhase == SPI_PHASE_2EDGE)
    {
      mode = SPI_MODE1;
    }
  }
  else if (_spi->Init.CLKPolarity == SPI_POLARITY_HIGH)
  {
    if (_spi->Init.CLKPhase == SPI_PHASE_1EDGE)
    {
      mode = SPI_MODE2;
    }
    else if (_spi->Init.CLKPhase = (uint32_t)SPI_PHASE_2EDGE)
    {
      mode = SPI_MODE3;
    }
  }
  else
  {
    mode = SPI_MODE3;
  }
  return mode;
}

void SPIClass::setClockDivider(uint32_t clockDiv) //W806  not use Div func
{
  // SPI_PARAM_LOCK();
  // _div = clockDiv;
  // spiSetClockDiv(_spi, _div);
  // SPI_PARAM_UNLOCK();
}

uint32_t SPIClass::getClockDivider() //W806  not use Div func
{
  // return spiGetClockDiv(_spi);
  return 0;
}


#endif