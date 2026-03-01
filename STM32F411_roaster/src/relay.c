#include "relay.h"
#include "GPIO.h"

void RELAY_init(RELAY_Handle *relay) { GPIO_init_output(relay->data); }

// Note: Pin set to 0 is relay on hence why we use GPIO_clear
void RELAY_on(RELAY_Handle *relay) { GPIO_clear(relay->data); }

// Note: Pin set to 1 is relay off hence why we use GPIO_set
void RELAY_off(RELAY_Handle *relay) { GPIO_set(relay->data); }
