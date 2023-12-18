#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom328p.h>
#define LED_BUILTIN (1 << PB5)
#define PUSH_BUTTON (1 << PD2)

ISR(INT0_vect) {                   // magic here: Interrupt Service Routine
  PORTB ^= LED_BUILTIN;
}

static void setup() {
  DDRB = LED_BUILTIN;
  DDRD &= !(PUSH_BUTTON);          // pin as input
  PORTD = PUSH_BUTTON;             // pull-up

  EIMSK = 1 << INT0;               // enable PD2/INT0 as an interrupt source
  EICRA = 1 << ISC01;              // Enable direction of interrupt on INT0, 
                                   // falling edge
}

int main() {
  setup();
  sei();                           // enable global interrupts, SREG register

  while (true) {
    // idle, do nothing
    _delay_ms(10000);
  }
}