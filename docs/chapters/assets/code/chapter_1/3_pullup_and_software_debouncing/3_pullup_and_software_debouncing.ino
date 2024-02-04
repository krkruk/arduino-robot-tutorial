static constexpr const uint8_t PUSH_BUTTON = 2;
static uint8_t builtinLedState = LOW;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, builtinLedState);
}

void loop() {
  uint8_t pushButtonState = digitalRead(PUSH_BUTTON);
  if (pushButtonState == LOW) {
    delay(50);                                    // simple programmatic debouncing
                                                  // You should measure the precise debouncing period
                                                  // with an oscilloscope. 50ms is just an educated guess
    pushButtonState = digitalRead(PUSH_BUTTON);

    if (pushButtonState == LOW) {
      builtinLedState ^= 1;                       // toggling the LED state
      digitalWrite(LED_BUILTIN, builtinLedState);

      while (digitalRead(PUSH_BUTTON) == LOW);    // blocking the program until
                                                  // the user releases the button
    }
  }
}
