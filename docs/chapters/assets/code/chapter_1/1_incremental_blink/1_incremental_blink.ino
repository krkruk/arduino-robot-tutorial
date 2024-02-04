static constexpr const uint8_t HARDWARE_LED = 8;
static constexpr const uint8_t DELAY_RATE = 100;
static constexpr const uint16_t MAX_DELAY = 3000;

static uint8_t hardwareLedState = HIGH;
static uint8_t hardwareLedDelayCounter = 1;

void setup() {
  pinMode(HARDWARE_LED, OUTPUT);
  digitalWrite(HARDWARE_LED, HIGH); 
}

void loop() {
  uint16_t delayValue = DELAY_RATE * hardwareLedDelayCounter;
  ++hardwareLedDelayCounter;
  if (delayValue > MAX_DELAY) {
    delayValue = DELAY_RATE;
    hardwareLedDelayCounter = 1;
    hardwareLedState = HIGH;
  }

  hardwareLedState ^= 1;
  digitalWrite(HARDWARE_LED, hardwareLedState); 
  delay(delayValue);
}