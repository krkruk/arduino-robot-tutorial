#include <math.h>

int pin = 0;
void setup() {

  DDRB = 0xff;        // all pins as output
  DDRC = 0xff;
  DDRD = 0xff;

  Serial.begin(115200);
  while(!Serial) continue;
}

void loop() {
  if (Serial.available() > 0) {
    String result = Serial.readStringUntil('\n');
    pin = result.toInt();                               // accept int only, 0 is an error value
  }
  
  if (pin != 0) {
    uint8_t pin_state = pin > 0 ? HIGH : LOW;
    digitalWrite(abs(pin), pin_state);
    Serial.print("PIN set to : ");
    Serial.println(pin, DEC);
  }
  pin = 0;
}
