#include <string.h>

static const constexpr uint8_t TXT_BUFFER_SIZE {64};
static char const TXT_BUFFER[TXT_BUFFER_SIZE] {'\0'};

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
  uint16_t value = random(1000);                    // generate a random number
  char random_char = map(value, 0, 1000, 32, 127);  // random visible ASCII character
  ++counter;

  sprintf(TXT_BUFFER, "%s #%d: value=%d, char=%c",  // define a string and substitutions
    HELLO_WORLD_LABELS[counter%3],                  // substitute %s with a random string
    counter,                                        // substitute first %d with the counter
    value,                                          // substitute second %d with a random number
    random_char);                                   // substitute %c with a character

  Serial.println(TXT_BUFFER);
  delay(1000);
}
