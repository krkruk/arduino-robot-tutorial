#include <AccelStepper.h>
#include <avr/interrupt.h>

static const AccelStepper wheel(AccelStepper::HALF4WIRE, 7, 5, 6, 4);

void setup() {
  // configure Timer2 as CTC at frequency 768Hz
  TCCR2A = (1<<COM2A1)   // Clear on compare match
           | (1<<WGM21); // CTC

  TCCR2B = (1<<CS22);    // prescaler = 64
  OCR2A = 162;           // OCR_nx = 162, as derived in calculations
  TIMSK2 = (1<<OCIE2A);  // enable interrupt on COMPA

  // steps per revolution: 2048
  wheel.setMaxSpeed(768);  // max steps per second,
                           // simply guesstimated
  wheel.setSpeed(768);
}

void loop() {
  delay(1000); 
}

ISR(TIMER2_COMPA_vect) {
  wheel.runSpeed();  // move a wheel by a step.
                     // try really hard not to block anything in `loop()`, see the docs
}