#ifndef LCD_H
#define LCD_H

#include "GPIO.h"
#include <stdint.h>

typedef struct {
  GPIO_Pin en;
  GPIO_Pin rs;
  GPIO_Pin data[8];
} LCD_Handle;

void LCD_init(LCD_Handle *lcd);
void LCD_command(LCD_Handle *lcd, uint8_t cmd);
void pulse_enable(LCD_Handle *lcd);
void LCD_clear(LCD_Handle *lcd);
void LCD_write_char(LCD_Handle *lcd, uint8_t c);
void LCD_print(LCD_Handle *lcd, char *s);

#endif // !LCD_H
