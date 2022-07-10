#include <Arduino.h>

#define ADC_RESOLUTION  ADC16BIT
#define ADC_CH1         PA1

void setup()
{
  //the analogReadResolution needn’t set,if you use 8bit adc
  analogReadResolution(ADC_RESOLUTION);
  pinMode(ADC_CH1, INPUT);
}

void loop()
{
#if (ADC_RESOLUTION == ADC8BIT)
  uint32_t value;
  float volt = 0;
  value = analogRead(ADC_CH1);
  volt = (value * 2.4) / 256;
  //this version Serial lib is not enable,but you could use printf on UART0
  printf("ADC val [%d], volt [%f]\r\n", value, volt);

#else if (ADC_RESOLUTION == ADC16BIT)
  uint32_t value;
  float volt = 0;
  value = analogRead(ADC_CH1);
  volt = (value * 2.4) / 76000;
  //this version Serial lib is not enable,but you could use printf on UART0
  printf("ADC val [%d], volt [%f]\r\n", value, volt);
#endif
  delay(1000);
}