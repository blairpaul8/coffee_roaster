#include "GPIO.h"
#include "lcd.h"
#include "relay.h"
#include "stm32f411xe.h"
#include <stdint.h>

void interrupt_init();
void EXTI0_IRQHandler(void) __attribute__((interrupt, used));

volatile int prev_state = 0;

RELAY_Handle relay;
GPIO_Pin B9;
int RELAY_STATUS = 0;

int main(void) {
  // LED
  B9 = (GPIO_Pin){GPIOB, 9};
  GPIO_init_output(B9);

  // Data Pin for Relay
  relay.data = (GPIO_Pin){GPIOB, 1};
  RELAY_init(&relay);
  RELAY_off(&relay);

  // Pin for EXTI0 interrupt
  GPIO_Pin B0 = {GPIOB, 0};
  GPIO_init_input(B0);

  // interrupt_init();

  LCD_Handle lcd = {.en = {GPIOA, 8},
                    .rs = {GPIOA, 9},
                    .data = {{GPIOA, 0},
                             {GPIOA, 1},
                             {GPIOA, 2},
                             {GPIOA, 3},
                             {GPIOA, 4},
                             {GPIOA, 5},
                             {GPIOA, 6},
                             {GPIOA, 7}}};

  LCD_init(&lcd);

  LCD_print(&lcd, "Hi Tucker!");

  while (1) {
    int curr_state = (B0.port->IDR & (1 << B0.pin)) == 0;

    if (curr_state && !prev_state) {
      GPIO_toggle(B9); // Toggle LED
      if (RELAY_STATUS) {
        RELAY_off(&relay);
        RELAY_STATUS = 0;
      } else {
        RELAY_on(&relay);
        RELAY_STATUS = 1;
      }
      delay_ms(20);
    }
  }
}

void interrupt_init() {
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  // Clear the EXTI0 resgister
  SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;

  // Set EXTI0 to the correct bitset for PB0 as the interrupt
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;

  EXTI->IMR |= (1 << 0);
  EXTI->FTSR |= (1 << 0);
  EXTI->PR |= (1 << 0);

  NVIC_SetPriority(EXTI0_IRQn, 1);
  NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void) {
  // Clear pending bits
  EXTI->PR |= (1 << 0);

  GPIO_toggle(B9);

  // Debug flag
}
