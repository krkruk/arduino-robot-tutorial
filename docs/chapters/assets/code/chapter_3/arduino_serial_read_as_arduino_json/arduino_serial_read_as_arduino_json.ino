#include <math.h>
#include <ArduinoJson.h>
#include <string.h>

enum class Operations {
  UNKNOWN_OP,
  ENABLE_PIN,
  DISABLE_PIN
};

void setup() {

  DDRB = 0xff;        // all pins as output
  DDRC = 0xff;
  DDRD = 0xff;

  Serial.begin(115200);
  while(!Serial) continue;
}

void loop() {
  JsonDocument doc;
  if (Serial.available() > 0) {
    auto error = deserializeJson(doc, Serial);
    if (!error) {
      uint8_t pin_state = LOW;
      const char *operation = doc["op"];
      Operations mode = strcmp(operation, "LED_ON") == 0
        ? Operations::ENABLE_PIN
        : strcmp(operation, "LED_OFF") == 0
          ? Operations::DISABLE_PIN
          : Operations::UNKNOWN_OP;

      int pin = doc["pin"];

      switch (mode) {
        case Operations::ENABLE_PIN:
          pin_state = HIGH;
          [[fallthrough]]
        case Operations::DISABLE_PIN:
          digitalWrite(pin, pin_state);
          Serial.print("Executed: ");
          serializeJson(doc, Serial);
          Serial.println();
          break;
        default: break;
      }
    }
  }
}
