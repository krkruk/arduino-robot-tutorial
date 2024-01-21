class Servo {
  public: 
  enum class SERVO_SELECTOR {
    SERVO_1,
    SERVO_2
  };
  static inline void setAngle(SERVO_SELECTOR servo, uint8_t angle) {
    uint16_t dutyCycle = map(angle, 0, 180, 540, 2540);

    switch (servo) {
      case SERVO_SELECTOR::SERVO_1:
        OCR1A = dutyCycle;
        break;
      case SERVO_SELECTOR::SERVO_2:
        OCR1B = dutyCycle;      // unused: COM1B1 is disabled
        break;
      default:
        break; 
    }
  }
};
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
  ICR1 = 20000;          // 50Hz 
}

void loop() {
  for (uint8_t angle = 0; angle <= 180; angle += 45) {
    Servo::setAngle(Servo::SERVO_SELECTOR::SERVO_1, angle);
    delay(1000);
  }
}