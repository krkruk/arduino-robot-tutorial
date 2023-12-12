#include <avr/io.h>
#include <avr/delay.h>

// This is the same hardware LED we introduced earlier!
#define HW_LED (1 << PB0)

int main() {

  /* setup() */
  DDRB = (1 << PB0);
  PORTB = (1 << PB0);  // light up the LED
  /* end: setup() */

  /* loop() */
  while (1) {
    PORTB ^= (1 << PB0);
    _delay_ms(1000);
  }
  /* end: loop() */
}