#ifndef __WM_GPIO_H__
#define __WM_GPIO_H__

#include "wm_hal.h"

#ifdef __cplusplus 
extern "C" {
#endif
typedef struct
{
	uint32_t Pin;
	uint32_t Mode;
	uint32_t Pull;
} GPIO_InitTypeDef;

typedef enum
{
	GPIO_PIN_RESET = 0,
	GPIO_PIN_SET
} GPIO_PinState;

#define GPIOA	((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB	((GPIO_TypeDef *)GPIOB_BASE)

#define GPIO_PIN_0	((uint32_t)0x00000001)
#define GPIO_PIN_1	((uint32_t)0x00000002)
#define GPIO_PIN_2	((uint32_t)0x00000004)
#define GPIO_PIN_3	((uint32_t)0x00000008)
#define GPIO_PIN_4	((uint32_t)0x00000010)
#define GPIO_PIN_5	((uint32_t)0x00000020)
#define GPIO_PIN_6	((uint32_t)0x00000040)
#define GPIO_PIN_7	((uint32_t)0x00000080)
#define GPIO_PIN_8	((uint32_t)0x00000100)
#define GPIO_PIN_9	((uint32_t)0x00000200)
#define GPIO_PIN_10	((uint32_t)0x00000400)
#define GPIO_PIN_11	((uint32_t)0x00000800)
#define GPIO_PIN_12	((uint32_t)0x00001000)
#define GPIO_PIN_13	((uint32_t)0x00002000)
#define GPIO_PIN_14	((uint32_t)0x00004000)
#define GPIO_PIN_15	((uint32_t)0x00008000)
#define GPIO_PIN_16	((uint32_t)0x00010000)
#define GPIO_PIN_17	((uint32_t)0x00020000)
#define GPIO_PIN_18	((uint32_t)0x00040000)
#define GPIO_PIN_19	((uint32_t)0x00080000)
#define GPIO_PIN_20	((uint32_t)0x00100000)
#define GPIO_PIN_21	((uint32_t)0x00200000)
#define GPIO_PIN_22	((uint32_t)0x00400000)
#define GPIO_PIN_23	((uint32_t)0x00800000)
#define GPIO_PIN_24	((uint32_t)0x01000000)
#define GPIO_PIN_25	((uint32_t)0x02000000)
#define GPIO_PIN_26	((uint32_t)0x04000000)
#define GPIO_PIN_27	((uint32_t)0x08000000)
#define GPIO_PIN_28	((uint32_t)0x10000000)
#define GPIO_PIN_29	((uint32_t)0x20000000)
#define GPIO_PIN_30	((uint32_t)0x40000000)
#define GPIO_PIN_31	((uint32_t)0x80000000)
#define GPIO_PIN_ALL	((uint32_t)0xFFFFFFFF)

#define GPIO_PIN_MASK	0xFFFFFFFF

#define GPIO_MODE_INPUT					0x01
#define GPIO_MODE_OUTPUT				0x02

#define GPIO_MODE_IT_RISING				0x87
#define GPIO_MODE_IT_FALLING			0x88
#define GPIO_MODE_IT_RISING_FALLING		0x89
#define GPIO_MODE_IT_HIGH_LEVEL			0x8a
#define GPIO_MODE_IT_LOW_LEVEL			0x8b

#define GPIO_NOPULL						0x12
#define GPIO_PULLUP						0x13
#define GPIO_PULLDOWN					0x14


/** option 1 of the io */
#define WM_IO_OPTION1               1
/** option 2 of the io */
#define WM_IO_OPTION2               2
/** option 3 of the io */
#define WM_IO_OPTION3               3
/** option 4 of the io */
#define WM_IO_OPTION4               4
/** option 5 of the io */
#define WM_IO_OPTION5               5
/** option 6 of the io */
#define WM_IO_OPTION6               6
/** option 7 of the io */
#define WM_IO_OPTION7               7

/* io option1 */
#define WM_IO_OPT1_I2C_DAT          WM_IO_OPTION1
#define WM_IO_OPT1_PWM1             WM_IO_OPTION1
#define WM_IO_OPT1_PWM2             WM_IO_OPTION1
#define WM_IO_OPT1_PWM3             WM_IO_OPTION1
#define WM_IO_OPT1_PWM4             WM_IO_OPTION1
#define WM_IO_OPT1_PWM5             WM_IO_OPTION1
#define WM_IO_OPT1_UART0_RXD        WM_IO_OPTION1
#define WM_IO_OPT1_UART0_TXD        WM_IO_OPTION1
#define WM_IO_OPT1_PWM_BRAKE        WM_IO_OPTION1
#define WM_IO_OPT1_I2S_M_EXTCLK     WM_IO_OPTION1
#define WM_IO_OPT1_SPI_M_DO         WM_IO_OPTION1
#define WM_IO_OPT1_SPI_M_DI         WM_IO_OPTION1
#define WM_IO_OPT1_SPI_M_CS         WM_IO_OPTION1
#define WM_IO_OPT1_SPI_M_CK         WM_IO_OPTION1
#define WM_IO_OPT1_I2S_S_RL         WM_IO_OPTION1
#define WM_IO_OPT1_I2S_S_SCL        WM_IO_OPTION1
#define WM_IO_OPT1_I2S_S_SDA        WM_IO_OPTION1
#define WM_IO_OPT1_I2S_M_RL         WM_IO_OPTION1
#define WM_IO_OPT1_I2S_M_SCL        WM_IO_OPTION1
#define WM_IO_OPT1_I2S_M_SDA        WM_IO_OPTION1
#define WM_IO_OPT1_JTAG_RST         WM_IO_OPTION1
#define WM_IO_OPT1_JTAG_TDO         WM_IO_OPTION1
#define WM_IO_OPT1_JTAG_TDI         WM_IO_OPTION1
#define WM_IO_OPT1_JTAG_TCK_SWDCK   WM_IO_OPTION1
#define WM_IO_OPT1_JTAG_TMS_SWDAT   WM_IO_OPTION1
#define WM_IO_OPT1_UART1_RXD        WM_IO_OPTION1
#define WM_IO_OPT1_UART1_TXD        WM_IO_OPTION1
#define WM_IO_OPT1_UART1_RTS        WM_IO_OPTION1
#define WM_IO_OPT1_UART1_CTS        WM_IO_OPTION1
#define WM_IO_OPT1_SDIO_DAT         WM_IO_OPTION1

/* io option2 */
#define WM_IO_OPT2_PWM1             WM_IO_OPTION2
#define WM_IO_OPT2_PWM2             WM_IO_OPTION2
#define WM_IO_OPT2_PWM3             WM_IO_OPTION2
#define WM_IO_OPT2_PWM4             WM_IO_OPTION2
#define WM_IO_OPT2_PWM5             WM_IO_OPTION2
#define WM_IO_OPT2_SPI_M_DO         WM_IO_OPTION2
#define WM_IO_OPT2_SPI_M_DI         WM_IO_OPTION2
#define WM_IO_OPT2_SPI_M_CS         WM_IO_OPTION2
#define WM_IO_OPT2_SPI_M_CK         WM_IO_OPTION2
#define WM_IO_OPT2_I2C_SCL          WM_IO_OPTION2
#define WM_IO_OPT2_I2S_M_EXTCLK     WM_IO_OPTION2
#define WM_IO_OPT2_UART1_RXD        WM_IO_OPTION2
#define WM_IO_OPT2_UART1_TXD        WM_IO_OPTION2
#define WM_IO_OPT2_UART1_RTS        WM_IO_OPTION2
#define WM_IO_OPT2_UART1_CTS        WM_IO_OPTION2
#define WM_IO_OPT2_I2C_DAT          WM_IO_OPTION2
#define WM_IO_OPT2_PWM_BRAKE        WM_IO_OPTION2
#define WM_IO_OPT2_UART0_RTS        WM_IO_OPTION2
#define WM_IO_OPT2_UART0_CTS        WM_IO_OPTION2
#define WM_IO_OPT2_SDIO_DAT         WM_IO_OPTION2
#define WM_IO_OPT2_HSPI_CK          WM_IO_OPTION2
#define WM_IO_OPT2_HSPI_INT         WM_IO_OPTION2
#define WM_IO_OPT2_HSPI_CS          WM_IO_OPTION2
#define WM_IO_OPT2_HSPI_DI          WM_IO_OPTION2
#define WM_IO_OPT2_HSPI_DO          WM_IO_OPTION2

/* io option3 */
#define WM_IO_OPT3_UART0_RXD        WM_IO_OPTION3
#define WM_IO_OPT3_UART0_TXD        WM_IO_OPTION3
#define WM_IO_OPT3_UART0_RTS        WM_IO_OPTION3
#define WM_IO_OPT3_UART0_CTS        WM_IO_OPTION3
#define WM_IO_OPT3_SPI_M_DO         WM_IO_OPTION3
#define WM_IO_OPT3_SPI_M_DI         WM_IO_OPTION3
#define WM_IO_OPT3_SPI_M_CS         WM_IO_OPTION3
#define WM_IO_OPT3_SDIO_CK          WM_IO_OPTION3
#define WM_IO_OPT3_SDIO_CMD         WM_IO_OPTION3
#define WM_IO_OPT3_SDIO_DAT         WM_IO_OPTION3

/* io option4 */
#define WM_IO_OPT4_I2S_M_MCLK       WM_IO_OPTION4
#define WM_IO_OPT4_I2S_M_RL         WM_IO_OPTION4
#define WM_IO_OPT4_I2S_M_SCL        WM_IO_OPTION4
#define WM_IO_OPT4_I2S_M_SDA        WM_IO_OPTION4

/* io option5 */
#define WM_IO_OPT5_GPIO             WM_IO_OPTION5

/* io option6 */
#define WM_IO_OPT6_ADC              WM_IO_OPTION6
#define WM_IO_OPT6_LCD_COM          WM_IO_OPTION6
#define WM_IO_OPT6_LCD_SEG          WM_IO_OPTION6

/* io option7 */
#define WM_IO_OPT7_TOUCH_SENSOR     WM_IO_OPTION7


#define IS_GPIO_ALL_INSTANCE(INSTANCE) (((INSTANCE) == GPIOA) || \
                                        ((INSTANCE) == GPIOB))
										
#define IS_GPIO_AF_INSTANCE(INSTANCE) IS_GPIO_ALL_INSTANCE(INSTANCE)

#define IS_GPIO_PIN_ACTION(ACTION) (((ACTION) == GPIO_PIN_RESET) || ((ACTION) == GPIO_PIN_SET))

#define IS_GPIO_PIN(PIN)           (((((uint32_t)PIN) & GPIO_PIN_MASK ) != 0x00u) && ((((uint32_t)PIN) & ~GPIO_PIN_MASK) == 0x00u))

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_MODE_INPUT)              ||\
                            ((MODE) == GPIO_MODE_OUTPUT)          	 ||\
                            ((MODE) == GPIO_MODE_IT_RISING)          ||\
                            ((MODE) == GPIO_MODE_IT_FALLING)         ||\
                            ((MODE) == GPIO_MODE_IT_RISING_FALLING)  ||\

#define IS_GPIO_PULL(PULL) (((PULL) == GPIO_NOPULL) || ((PULL) == GPIO_PULLUP) || \
                            ((PULL) == GPIO_PULLDOWN))
							

void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);
void HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin);
void HAL_GPIO_Option(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint8_t GPIO_Opt);

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, GPIO_PinState PinState);
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
void HAL_GPIO_EXTI_IRQHandler(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
void HAL_GPIO_EXTI_Callback(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
#ifdef __cplusplus
}
#endif

#endif
