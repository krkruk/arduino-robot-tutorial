#define COIL_1 (1<<PD7)
#define COIL_2 (1<<PD6)
#define COIL_3 (1<<PD5)
#define COIL_4 (1<<PD4)

#define DELAY_MS 2

void move_stepper_half_step_naive() {
  PORTD = COIL_1;
  _delay_ms(DELAY_MS);

  PORTD |= COIL_2;
  _delay_ms(DELAY_MS);

  PORTD = COIL_2;
  _delay_ms(DELAY_MS);

  PORTD |= COIL_3;
  _delay_ms(DELAY_MS);

  PORTD = COIL_3;
  _delay_ms(DELAY_MS);

  PORTD |= COIL_4;
  _delay_ms(DELAY_MS);

  PORTD = COIL_4;
  _delay_ms(DELAY_MS);

  PORTD |= COIL_1;
  _delay_ms(DELAY_MS);
}

void move_stepper_half_step_clockwise_smartish() {
  uint8_t previous_step = PORTD;
  PORTD = COIL_1;
  while (!(PORTD & COIL_4)) {
    previous_step = PORTD;
    _delay_ms(DELAY_MS);
    PORTD = (PORTD>>1) | PORTD;
    _delay_ms(DELAY_MS);
    PORTD &= ~(previous_step);
  }
  _delay_ms(DELAY_MS);
  PORTD |= COIL_1;
  _delay_ms(DELAY_MS);
}

void move_stepper_half_step_counter_clockwise_smartish() {
  uint8_t previous_step = PORTD;
  PORTD = COIL_4;
  while (!(PORTD & COIL_1)) {
    previous_step = PORTD;
    _delay_ms(DELAY_MS);
    PORTD = (PORTD<<1) | PORTD;
    _delay_ms(DELAY_MS);
    PORTD &= ~(previous_step);
  }
  _delay_ms(DELAY_MS);
  PORTD |= COIL_4;
  _delay_ms(DELAY_MS);
}

int main() {
  DDRD = COIL_1 | COIL_2 | COIL_3 | COIL_4;
  PORTD = 0x00;
  while (true) {
      move_stepper_half_step_clockwise_smartish();      
  }
}
