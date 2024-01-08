static constexpr const uint8_t ANALOG_PIN_PHOTORESISTOR = A0; 

static uint16_t photoresistorValue = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
}

void loop() {
  photoresistorValue = analogRead(ANALOG_PIN_PHOTORESISTOR);

  Serial.print("photoresistor ADC (0-1023) = ");
  Serial.println(photoresistorValue);

  delay(50);
}
