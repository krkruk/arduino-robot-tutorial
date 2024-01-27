#include <ArduinoJson.h>

JsonDocument json;

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

void applyChassisCommand(tuple<ChassisCommands::ChassisCmdEnum, uint8_t> cmd) {
  // do chassis thing
}

void applyServoCommand(tuple<SledgehammerCommands::SledgehammerCmdEnum, uint8_t> cmd) {
  // do servo thing
}

void setup() {
  Serial.begin(115200);
  while (!Serial);


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
}
