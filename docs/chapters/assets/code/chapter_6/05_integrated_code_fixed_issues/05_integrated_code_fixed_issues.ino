#include <ArduinoJson.h>
#include <AccelStepper.h>
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

template<class K, class V>
class tuple {
  public:
  K key;
  V value;
  tuple(K key, V value) : key{key}, value{value} {}
};

class ChassisCommands{
  public:
  static constexpr const char * KEY_FORWARDS = "FWD";
  static constexpr const char * KEY_BACKWARDS = "BWD";
  static constexpr const char * KEY_TURN_LEFT = "T_L";
  static constexpr const char * KEY_TURN_RIGHT = "T_R";
  static constexpr const char * KEY_STOP = "STOP";

  enum ChassisCmdEnum {
    UNDEFINED,
    FORWARDS,
    BACKWARDS,
    TURN_LEFT,
    TURN_RIGHT,
    STOP
  };

  static tuple<ChassisCmdEnum, uint8_t> convert(const JsonDocument &doc) {
    if (doc.containsKey(KEY_FORWARDS)) {
      return tuple<ChassisCmdEnum, uint8_t>(FORWARDS, doc[KEY_FORWARDS].as<uint8_t>());
    } else if (doc.containsKey(KEY_BACKWARDS)) {
      return tuple<ChassisCmdEnum, uint8_t>(BACKWARDS, doc[KEY_BACKWARDS].as<uint8_t>());
    } else if (doc.containsKey(KEY_TURN_LEFT)) {
      return tuple<ChassisCmdEnum, uint8_t>(TURN_LEFT, doc[KEY_TURN_LEFT].as<uint8_t>());
    } else if (doc.containsKey(KEY_TURN_RIGHT)) {
      return tuple<ChassisCmdEnum, uint8_t>(TURN_RIGHT, doc[KEY_TURN_RIGHT].as<uint8_t>());
    } else if (doc.containsKey(KEY_STOP)) {
      return tuple<ChassisCmdEnum, uint8_t>(STOP, doc[KEY_STOP].as<uint8_t>());
    }
    return tuple<ChassisCmdEnum, uint8_t>(UNDEFINED, 0);
  }
};

class SledgehammerCommands{
  public:
  static constexpr const char * KEY_HIT = "HIT";
  static constexpr const char * KEY_SET_SERVO_POSITION = "SPOS";
  static constexpr const char * KEY_STOP = "STOP";

  enum SledgehammerCmdEnum {
    UNDEFINED,
    HIT,
    SET_SERVO_POSITION,
    STOP
  };

  static tuple<SledgehammerCmdEnum, uint8_t> convert(const JsonDocument &doc) {
    if (doc.containsKey(KEY_HIT)) {
      return tuple<SledgehammerCmdEnum, uint8_t>(HIT, 0);
    } else if (doc.containsKey(KEY_SET_SERVO_POSITION)) {
      return tuple<SledgehammerCmdEnum, uint8_t>(SET_SERVO_POSITION, doc[KEY_SET_SERVO_POSITION].as<uint8_t>());
    } else if (doc.containsKey(KEY_STOP)) {
      return tuple<SledgehammerCmdEnum, uint8_t>(STOP, doc[KEY_STOP].as<uint8_t>());
    }
    return tuple<SledgehammerCmdEnum, uint8_t>(UNDEFINED, 0);
  }
};

void applyChassisCommand(tuple<ChassisCommands::ChassisCmdEnum, uint8_t> cmd);
void applyServoCommand(tuple<SledgehammerCommands::SledgehammerCmdEnum, uint8_t> cmd);

static JsonDocument json;
static WeaponizedServo sledgehammer;
static Chassis chassis;

void setup() {
  Serial.setTimeout(20);
  Serial.begin(115200);
  while (!Serial);
  sledgehammer.init();
}

void loop() {
  if (Serial.available()) {
    auto error = deserializeJson(json, Serial);
    if (!error) {
      auto chassisCmd = ChassisCommands::convert(json);
      auto servoCmd = SledgehammerCommands::convert(json);

      // business here 
      applyChassisCommand(chassisCmd);
      applyServoCommand(servoCmd);

      // prepare response
      json["chassisCmd"] = static_cast<int>(chassisCmd.key);
      json["servoCmd"] = static_cast<int>(servoCmd.key);
      serializeJson(json, Serial);      
      Serial.println();
      json.clear();
    }
  }

  chassis.run();
  WeaponizedServo::tick();
}

void applyChassisCommand(tuple<ChassisCommands::ChassisCmdEnum, uint8_t> cmd) {
  switch (cmd.key) {
    case ChassisCommands::FORWARDS:
      chassis.driveForwards(cmd.value);
      break;
    case ChassisCommands::BACKWARDS:
      chassis.driveBackwards(cmd.value);
      break;
    case ChassisCommands::TURN_LEFT:
      chassis.turnLeft(cmd.value);
      break;
    case ChassisCommands::TURN_RIGHT:
      chassis.turnRight(cmd.value);
      break;
    case ChassisCommands::STOP:
      chassis.stop();
      break;
    default:
      // unsupported operation
      [[fallthrough]];
  }
}

void applyServoCommand(tuple<SledgehammerCommands::SledgehammerCmdEnum, uint8_t> cmd) {
  switch (cmd.key) {
  case SledgehammerCommands::HIT:
    if (sledgehammer) {
      sledgehammer.hit();
    }
    break;
  case SledgehammerCommands::SET_SERVO_POSITION:
    sledgehammer.changePosition(cmd.value);
    break;
  case SledgehammerCommands::STOP:
    sledgehammer.defaultPosition();
    break;
  default:
    // unsupported operation
    [[fallthrough]];
  }
}