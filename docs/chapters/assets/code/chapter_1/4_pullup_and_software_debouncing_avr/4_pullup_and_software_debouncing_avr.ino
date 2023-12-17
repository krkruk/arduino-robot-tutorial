#define LED_BUILTIN (1 << PB5)
#define PUSH_BUTTON (1 << PD2)

int main() {

  // setup()
  DDRB = LED_BUILTIN;
  DDRD = 0x00;          //set the enitre port as input - including PD2

  PORTB = 0x00;
  PORTD = PUSH_BUTTON;  //PD2 is set as input and the state as HIGH - pullup enabled

  // loop()
  while (true) {

    // the push button is pressed if it's state is 0

      if (bit_is_clear(PIND, PD2)) {  // much clearer than: !(PIND & (1<<PD2))
        _delay_ms(50);
        if (!(PIND & PUSH_BUTTON)) {
          PORTB ^= LED_BUILTIN;
          while (bit_is_clear(PIND, PD2));
        }
      }
  }
}