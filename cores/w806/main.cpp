#include <stdio.h>
#include <Arduino.h>
#include "wm_hal.h"

void Error_Handler(void);

int main(void)
{
#if (F_CPU == 240000000)
    uint32_t f_cpu_dir = (uint32_t)CPU_CLK_240M;
#elif (F_CPU == 80000000)
    uint32_t f_cpu_dir = (uint32_t)CPU_CLK_80M;
#elif (F_CUP == 4000000)
    uint32_t f_cpu_dir = (uint32_t)CPU_CLK_40M;
#elif (F_CUP == 2000000)
    uint32_t f_cpu_dir = (uint32_t)CPU_CLK_2M;
#else
    uint32_t f_cpu_dir = CPU_CLK_160M;
#endif

    SystemClock_Config(f_cpu_dir);
    init();

    setup();
    
    for (;;) {
        loop();
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
