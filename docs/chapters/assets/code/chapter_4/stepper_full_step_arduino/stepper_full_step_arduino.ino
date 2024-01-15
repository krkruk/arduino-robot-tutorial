#define COIL_1 7
#define COIL_2 6
#define COIL_3 5
#define COIL_4 4

#define DELAY_MS 4
void clearStepperPins() {
  digitalWrite(COIL_1, LOW);
  digitalWrite(COIL_2, LOW);
  digitalWrite(COIL_3, LOW);
  digitalWrite(COIL_4, LOW);
}

void setup() {
  pinMode(COIL_1, OUTPUT);
  pinMode(COIL_2, OUTPUT);
  pinMode(COIL_3, OUTPUT);
  pinMode(COIL_4, OUTPUT);
}

void loop() {
  clearStepperPins();
  digitalWrite(COIL_1, HIGH);
  delay(DELAY_MS);

  clearStepperPins();
  digitalWrite(COIL_2, HIGH);
  delay(DELAY_MS);

  clearStepperPins();
  digitalWrite(COIL_3, HIGH);
  delay(DELAY_MS);

  clearStepperPins();
  digitalWrite(COIL_4, HIGH);
  delay(DELAY_MS);
}
