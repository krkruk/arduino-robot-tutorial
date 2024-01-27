#include <Servo.h>
#include <arduino-timer.h>

class WeaponizedServo {
  const uint8_t SERVO_PIN;
  const uint16_t SERVO_0DEG_MICROS;
  const uint16_t SERVO_180DEG_MICROS;
  Servo servo;

protected:
  static Timer<2> timer;
public:
  WeaponizedServo(uint8_t servoPin = 9, uint16_t deg0Micros=540, uint16_t deg180Micros=2540) 
    : SERVO_PIN{servoPin},
      SERVO_0DEG_MICROS{deg0Micros},
      SERVO_180DEG_MICROS{deg180Micros} {}

  void init() {
    servo.attach(SERVO_PIN, SERVO_0DEG_MICROS, SERVO_180DEG_MICROS);
    defaultPosition();
  }

  void changePosition(uint8_t angle) {
    angle = min(angle, 180);
    servo.write(angle);
  }

  void defaultPosition() {
    servo.write(45);
  } 

  void hit() {
    // servo is callibrated to 'hit' at 180deg angle
    servo.write(180);
    WeaponizedServo::timer.in(350, [](void* thizz) -> bool { 
      static_cast<WeaponizedServo*>(thizz)->defaultPosition();
      return false; 
    }, this);
  }

  static void tick() {
    WeaponizedServo::timer.tick();
  }

  /*
  * Return true if the servo is ready to perform another hit.
  */
  operator bool() const {
    return WeaponizedServo::timer.empty();
  }

};
// initialize static timer
Timer<2> WeaponizedServo::timer;


WeaponizedServo sledgehammer;

void setup() {
  sledgehammer.init();
}

void loop() {
  if (sledgehammer) {
    delay(2000);
    sledgehammer.hit();
  }
  WeaponizedServo::tick();
}
