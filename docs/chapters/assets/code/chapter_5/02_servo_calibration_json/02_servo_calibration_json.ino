#include <Servo.h>
#include <ArduinoJson.h>

static constexpr const uint8_t SERVO_PIN {9};
static Servo servo;

static JsonDocument json;

inline void applyServoPosition(uint8_t angle);

void setup() {
  Serial.begin(115200);
  servo.attach(SERVO_PIN, 540, 2540);       // calibrated servo  
  servo.write(0);

  while (!Serial);
}

void loop() {
  if (Serial.available() > 0) {
    auto error = deserializeJson(json, Serial);
    if (!error) {
      applyServoPosition(
        static_cast<uint8_t>(json["pos"])); // contract: pos shall be in range 0-180
      serializeJson(json, Serial);
      Serial.println();
    }
  }
}

void applyServoPosition(uint8_t angle) {
  servo.write(angle);
}