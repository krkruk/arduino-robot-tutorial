static constexpr const uint8_t DUTY_CYCLE = 10;

static constexpr const int PERIOD_MS = 10;                              // 1/0.01s = 100Hz
static constexpr const int ON_PERIOD_MS = PERIOD_MS * DUTY_CYCLE / 100; // round
static constexpr const int OFF_PERIOD_MS = PERIOD_MS - ON_PERIOD_MS;

static constexpr const uint8_t LED_PIN = LED_BUILTIN;


void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  
  delay(ON_PERIOD_MS);  

  digitalWrite(LED_PIN, LOW);   
  delay(OFF_PERIOD_MS);                      
}