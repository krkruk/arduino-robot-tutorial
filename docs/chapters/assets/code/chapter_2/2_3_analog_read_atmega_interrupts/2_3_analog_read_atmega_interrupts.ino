volatile uint16_t photoresistorValue = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  ADCSRA = (1 << ADEN)  // enable ADC
    | ((1 << ADPS2) 
    | (1 << ADPS1) 
    | (1 << ADPS0))     // prescaler 128
    | (1 << ADIE);      // enable interrupts

  ADMUX = (1 << REFS0); // MUX3..0 bits are set to 0, which is ADC0/PC0/A0 pin
  ADCSRA |= 1 << ADSC;  // start ADC, handle a response in the ISR routine
  sei();
}

void loop() {  
  Serial.print("photoresistor ADC (0-1023) = ");
  Serial.println(photoresistorValue);

  delay(50);
}

ISR(ADC_vect) {
  photoresistorValue = ADC;
  ADCSRA |= 1 << ADSC;      // run ADC/ch:adc0 immediately again
}
