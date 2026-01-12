#include "GPIO.h"
#include "lcd.h"
#include "stm32f411xe.h"
#include <stdint.h>

int main(void) {
  // LED
  GPIO_Pin B9 = {GPIOB, 9};
  GPIO_init_output(B9);

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
    GPIO_toggle(B9);
    delay_ms(1000);
  }
}
