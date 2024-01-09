static constexpr const uint8_t DUTY_CYCLE = 10;  // 10% duty cycle
static constexpr const uint8_t LED_PIN = 6;      // PWM-ready pins: 3, 5, 6, 9, 10, 11

static constexpr const uint8_t PWM_VALUE = UINT8_MAX * DUTY_CYCLE / 100;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, PWM_VALUE);
}

void loop() {
  // nothing to do             
}