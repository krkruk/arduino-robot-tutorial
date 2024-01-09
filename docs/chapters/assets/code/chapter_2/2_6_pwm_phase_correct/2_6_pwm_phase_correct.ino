static constexpr const uint8_t DUTY_CYCLE = 10;  // 10% duty cycle

void setup() {
  DDRB |= (1<<PB1); // Set PB1 as output
  /*
      Enable PWM on PIN PB1/Arduin D9

      CPU CLK, fclk_I/O = 16,000,000
      Timer: Phase Correct PWM, see 15.9.4 Phase Correct PWM Mode in Datasheet
      Formula: fOCnxPCPWM = fclk_I/O / {2* N * TOP}
      fOCnxPCPWM=50Hz
      Prescaler: N=8
      TOP = fclk_I/O / {2 * N * fOCnxPCPWM}

      TOP=ICR1=20000
  */
  TCCR1A = (1<<COM1A1)   // PB1 as PWM, see the pinout
      | (1<<WGM11);      // phase correct PWM, TOP=ICR1
  TCCR1B = (1 << WGM13) 
      | (1 << CS11);     // prescaler 8
  ICR1 = 20000;

  OCR1A = ICR1 * DUTY_CYCLE / 100;  // PWM values are in range 0..ICR1
}

void loop() {
  // nothing to do
}
