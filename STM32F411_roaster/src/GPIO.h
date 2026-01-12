/******************************************
 *   Basic GPIO Library for stm32f411xe   *
 * ****************************************/

#ifndef GPIO_H
#define GPIO_H

#include "stm32f411xe.h"
#include <stdint.h>

typedef struct {
  GPIO_TypeDef *port;
  uint8_t pin;
} GPIO_Pin;

void GPIO_init_output(GPIO_Pin pin);
void GPIO_set(GPIO_Pin pin);
void GPIO_clear(GPIO_Pin pin);
void GPIO_toggle(GPIO_Pin pin);
void delay_us(volatile uint32_t us);
void delay_ms(volatile uint32_t ms);

#endif // !GPIO_H
