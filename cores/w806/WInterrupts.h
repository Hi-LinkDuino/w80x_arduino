#ifndef __WM_IT_H__
#define __WM_IT_H__
#ifdef __cplusplus 
extern "C" {
#endif
void CORET_IRQHandler(void);
void GPIOA_IRQHandler(void);
void GPIOB_IRQHandler(void);
void UART0_IRQHandler(void);
void UART1_IRQHandler(void);
// void UART2_4_IRQHandler(void);
void UART2_5_IRQHandler(void);                  //TODO fixbug this is uart2-5 not 2-4
void WDG_IRQHandler(void);
void TIM0_5_IRQHandler(void);
void ADC_IRQHandler(void);
void PMU_IRQHandler(void);
void TOUCH_IRQHandler(void);
#ifdef __cplusplus 
}
#endif
#endif