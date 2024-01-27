#include <AccelStepper.h>

class Chassis {
  const uint16_t MAX_SPEED;
  const AccelStepper rightWheel; // default pinout:  (7, 5, 6, 4)
  const AccelStepper leftWheel; //  default pinout: (PIN_A4, PIN_A2, PIN_A3, PIN_A1)

public:
  Chassis(uint16_t maxSpeed = 768,
    uint8_t rPin0= 7, uint8_t rPin1 = 5, uint8_t rPin2 = 6, uint8_t rPin3 = 4,
    uint8_t lPin0 = PIN_A4, uint8_t lPin1 = PIN_A2, uint8_t lPin2= PIN_A3, uint8_t lPin3 = PIN_A1)
    : MAX_SPEED(maxSpeed),
      rightWheel(AccelStepper::HALF4WIRE, rPin0, rPin1, rPin2, rPin3),
      leftWheel(AccelStepper::HALF4WIRE, lPin0, lPin1, lPin2, lPin3) {
        rightWheel.setMaxSpeed(MAX_SPEED);
        leftWheel.setMaxSpeed(MAX_SPEED);
  }

  void driveForwards(uint8_t speed) {
    auto stepperSpeed = 1 * mapSpeed(speed);
    rightWheel.setSpeed(stepperSpeed);
    leftWheel.setSpeed(stepperSpeed);
  }
  
  void driveBackwards(uint8_t speed) {
    auto stepperSpeed = -1 * mapSpeed(speed);;
    rightWheel.setSpeed(stepperSpeed);
    leftWheel.setSpeed(stepperSpeed);
  }

  void turnRight(uint8_t speed) {
    auto stepperSpeed = 1 * mapSpeed(speed);
    rightWheel.setSpeed(-stepperSpeed);
    leftWheel.setSpeed(stepperSpeed);
  }

  void turnLeft(uint8_t speed) {
    auto stepperSpeed = 1 * mapSpeed(speed);
    rightWheel.setSpeed(stepperSpeed);
    leftWheel.setSpeed(-stepperSpeed);
  }

  void stop() {
    rightWheel.setSpeed(0);
    leftWheel.setSpeed(0);
  }

  void run() {
    rightWheel.runSpeed();
    leftWheel.runSpeed();
  }

private:
  int16_t mapSpeed(uint8_t speed) const {
    return map(speed, 0, 255, 0, MAX_SPEED);
  }
};

Chassis chassis;

void setup() {
  // put your setup code here, to run once:
  chassis.driveForwards(200);
}

void loop() {
  chassis.run();
}
