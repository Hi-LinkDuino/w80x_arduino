#include <stdio.h>
#include <Arduino.h>
#include "wm_hal.h"

void Error_Handler(void);
//static void GPIO_Init(void);

static volatile uint8_t key_flag = 0;

int main(void)
{
	SystemClock_Config(CPU_CLK_160M);
//	HAL_Init();
//	GPIO_Init();
    setup();
    
	for (;;) {
		loop();
/*        HAL_Delay(500);
		key_flag++;
		
		if(key_flag%3==0)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (GPIO_PinState)0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (GPIO_PinState)1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (GPIO_PinState)1);
		}
		else if(key_flag%3==1)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (GPIO_PinState)1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (GPIO_PinState)0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (GPIO_PinState)1);
		}
		else
		{

			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (GPIO_PinState)1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (GPIO_PinState)1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (GPIO_PinState)0);
		}
    */

	}

    return 0;
}

void Error_Handler(void)
{
	while (1)
	{
	}
}

void assert_failed(uint8_t *file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
