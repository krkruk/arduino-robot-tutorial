static uint16_t photoresistorValue = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  ADCSRA = (1 << ADEN)  // enable ADC
    | ((1 << ADPS2) 
    | (1 << ADPS1) 
    | (1 << ADPS0));     // prescaler 128
  ADMUX = (1 << REFS0); // MUX3..0 bits are set to 0, which is ADC0/PC0/A0 pin
}

void loop() {  
  ADCSRA |= (1 << ADSC);            // start ADC convertion
  while (!(ADCSRA & (1 << ADIF)));  // wait until ADC completes its job
  photoresistorValue = ADC;         // provide ADC value present on A0 pin

  Serial.print("photoresistor ADC (0-1023) = ");
  Serial.println(photoresistorValue);

  delay(50);
}
