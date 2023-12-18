static constexpr uint8_t PUSH_BUTTON = 2;
volatile uint8_t builtinLedState = LOW;


void toggleLed() {
  builtinLedState ^= 1;                       // toggling the LED state
  digitalWrite(LED_BUILTIN, builtinLedState);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, builtinLedState);
  attachInterrupt(                            // enabling interrupts on D2
    digitalPinToInterrupt(PUSH_BUTTON),       // mapping D2 to Atmega's INT0
                                              // this is for readability
    &toggleLed,                                // a pointer to function, simply pass
                                              // a name of your function, it's not that scary!
    FALLING);                                 // activate the interrupt on falling edge
}

void loop() {
  delay(10000);
}