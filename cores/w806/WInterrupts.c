#include "wm_hal.h"

extern ADC_HandleTypeDef hadc;

#define readl(addr) ({unsigned int __v = (*(volatile unsigned int *) (addr)); __v;})
__attribute__((isr)) void CORET_IRQHandler(void)
{
	readl(0xE000E010);
	HAL_IncTick();
}
__attribute__((isr)) void GPIOA_IRQHandler(void)
{
 	HAL_GPIO_EXTI_IRQHandler(GPIOA, GPIO_PIN_0);
}

__attribute__((isr)) void GPIOB_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIOB, GPIO_PIN_5);
}

__attribute__((isr)) void ADC_IRQHandler(void)
{
 	HAL_ADC_IRQHandler(&hadc);
}