#include "lcd.h"
#include "GPIO.h"

#include <stdint.h>
#include <string.h>

/*********************
 *    LCD Commands   *
 *********************/
#define CLEAR 0x01
#define ON 0x0F
#define OFF 0x08
#define ENTRY_MODE 0x06
#define FUNCTION_SET 0x38

// Pulse the enable bit on the LCD1602
void pulse_enable(LCD_Handle *lcd) {

  GPIO_set(lcd->en);

  // delay 2 microseconds
  delay_us(2);

  GPIO_clear(lcd->en);

  // delay 2 milliseconds
  delay_ms(2);
}

void LCD_command(LCD_Handle *lcd, uint8_t cmd) {
  GPIO_clear(lcd->rs);

  for (uint8_t i = 0; i < 8; i++) {
    // if the bit is 1 in the command Set the data line to high
    if (cmd & (1 << i)) {
      GPIO_set(lcd->data[i]);
    } else {
      GPIO_clear(lcd->data[i]);
    }
  }

  pulse_enable(lcd);
}

void LCD_init(LCD_Handle *lcd) {
  // Initialize GPIO pins
  GPIO_init_output(lcd->en);
  GPIO_init_output(lcd->rs);
  // Initialize data lines
  for (int i = 0; i < 8; i++) {
    GPIO_init_output(lcd->data[i]);
  }

  GPIO_clear(lcd->en);
  GPIO_clear(lcd->rs);

  for (int i = 0; i < 8; i++) {
    GPIO_clear(lcd->data[i]);
  }

  delay_ms(20);

  LCD_command(lcd, FUNCTION_SET);
  delay_ms(5);

  LCD_command(lcd, OFF);
  delay_ms(5);

  LCD_command(lcd, CLEAR);
  delay_ms(2);
  LCD_command(lcd, ENTRY_MODE);
  delay_ms(5);

  LCD_command(lcd, ON);
}

void LCD_clear(LCD_Handle *lcd) { LCD_command(lcd, CLEAR); }

void LCD_write_char(LCD_Handle *lcd, uint8_t c) {
  GPIO_set(lcd->rs);
  for (int i = 0; i < 8; i++) {
    if (c & (1 << i)) {
      GPIO_set(lcd->data[i]);
    } else {
      GPIO_clear(lcd->data[i]);
    }
  }

  pulse_enable(lcd);

  GPIO_clear(lcd->rs);
  delay_ms(2);
}

void LCD_print(LCD_Handle *lcd, char *s) {
  for (int i = 0; i < strlen(s); i++) {
    LCD_write_char(lcd, s[i]);
  }
}
