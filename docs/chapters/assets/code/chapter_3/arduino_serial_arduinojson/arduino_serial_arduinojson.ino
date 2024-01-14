#include <ArduinoJson.h>
//ArduinoJson: 7.0.0

static int counter {0};
static const char *HELLO_WORLD_LABELS[] = {
  "Hello World",
  "HELLO WORLD",
  "HeLlO WoRlD!"
};

void setup() {
  Serial.begin(115200);
  while(!Serial) continue;
  randomSeed(analogRead(0));
}

void loop() {
  float value = random(1000) / 100.0;                    // generate a random number
  ++counter;

  JsonDocument doc;                                 // Dynamic memory allocation with malloc/free
  doc["str"] = HELLO_WORLD_LABELS[counter%3];       // Assign one of 3 strings dynamically
  doc["cntr"] = counter;                            // Assign cntr=counter
  doc["val"] = value;                               // Assign val=value
  //doc["r_char"] = random_char;                    // fail: must be converted to string

  serializeJson(doc, Serial);
  Serial.println();
  
  delay(1000);
}
