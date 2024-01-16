#include <AccelStepper.h>

static const AccelStepper wheel(AccelStepper::HALF4WIRE, 7, 5, 6, 4);

void setup() {
                            // steps per revolution: 2048
  wheel.setMaxSpeed(768);   // max steps per second,
                            // simply guesstimated
  wheel.setSpeed(768);
}

void loop() {
  wheel.runSpeed(); // move a wheel by a step.
                    // try really hard not to block anything in `loop()`, see the docs
}
