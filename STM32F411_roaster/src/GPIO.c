#include "GPIO.h"
#include <sys/_intsup.h>

void GPIO_init_output(GPIO_Pin pin) {
  if (pin.port == GPIOA) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  } else {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  }

  pin.port->MODER &= ~(0b11 << (pin.pin * 2));
  pin.port->MODER |= (0b01 << (pin.pin * 2));

  pin.port->OTYPER &= ~(1 << pin.pin);

  pin.port->OSPEEDR &= ~(0b11 << (pin.pin * 2));
  pin.port->OSPEEDR |= (0b01 << (pin.pin * 2));

  pin.port->PUPDR &= ~(0b11 << (pin.pin * 2));
}

void GPIO_toggle(GPIO_Pin pin) { pin.port->ODR ^= (1 << pin.pin); }
void GPIO_clear(GPIO_Pin pin) { pin.port->ODR &= ~(1 << pin.pin); }
void GPIO_set(GPIO_Pin pin) { pin.port->ODR |= (1 << pin.pin); }

void delay_us(volatile uint32_t us) {
  while (us--) {
    __asm__("nop");
  }
}

void delay_ms(volatile uint32_t ms) {
  while (ms--) {
    delay_us(1000);
  }
}
