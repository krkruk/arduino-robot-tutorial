#include <Servo.h>

static constexpr const uint8_t SERVO_PIN {9};
static Servo servo;

void setup() {
  servo.attach(SERVO_PIN);
}

void loop() {
  for (uint8_t angle = 0; angle <= 180; angle += 45) {
    servo.write(angle);
    delay(1000);
  }
}
