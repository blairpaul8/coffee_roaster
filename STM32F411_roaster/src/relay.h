#ifndef RELAY_H
#define RELAY_H

#include "GPIO.h"

typedef struct {
  GPIO_Pin data;
} RELAY_Handle;

void RELAY_init(RELAY_Handle *relay);
void RELAY_on(RELAY_Handle *relay);
void RELAY_off(RELAY_Handle *relay);

#endif // !RELAY_H
