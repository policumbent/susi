#include <Arduino.h>
#include <BfButton.h>

#include "Calibration.h"
#include "Gear.h"
#include "TServo.h"

auto btn_shift_up = BfButton(BfButton::STANDALONE_DIGITAL, 12, true, LOW);
auto btn_shift_down = BfButton(BfButton::STANDALONE_DIGITAL, 14, true, LOW);
auto servo1 = TServo{26, 80, false, 700, 2191};
auto servo2 = TServo{27, 25, true, 800, 2200};

auto vec_servo1 = GearVec{};
auto vec_servo2 = GearVec{};
auto cal = Calibration{&vec_servo1, &vec_servo2};

uint8_t current_gear_id = 0;

void IRAM_ATTR btn_callback(BfButton *btn, BfButton::press_pattern_t pattern) {
  if (btn->getID() == btn_shift_down.getID() &&
      pattern == BfButton::SINGLE_PRESS && current_gear_id > MIN_GEAR) {

    auto id = --current_gear_id;
    auto pos1 = vec_servo1[id - 1].shift_down();
    auto pos2 = vec_servo2[id - 1].shift_down();

    servo1.move_to(pos1);
    servo2.move_to(pos2);

    Memory::save_gear(MemGear{id, pos1, pos2});
  } else if (btn->getID() == btn_shift_up.getID() &&
             pattern == BfButton::SINGLE_PRESS && current_gear_id < MAX_GEAR) {

    auto id = ++current_gear_id;
    auto pos1 = vec_servo1[id - 1].shift_up();
    auto pos2 = vec_servo2[id - 1].shift_up();

    servo1.move_to(pos1);
    servo2.move_to(pos2);

    Memory::save_gear(MemGear{id, pos1, pos2});
  } else if (btn->getID() == btn_shift_up.getID() && pattern == BfButton::LONG_PRESS) {
    cal.isActive() ? cal.end() : cal.begin();
  }
}

void setup() {
  // debouncing
  delay(500);

  // button logic
  btn_shift_up.onPress(btn_callback).onPressFor(btn_callback, 3000);
  btn_shift_down.onPress(btn_callback);

  // retrieve configuration
  vec_servo1 = Memory::load_config_servo1();
  vec_servo2 = Memory::load_config_servo2();

  // retrieve last gear
  auto last_gear = Memory::load_gear();
  current_gear_id = last_gear.id;

  // retrieve last position of the servo
  servo1.move_to(last_gear.pos_servo1);
  servo2.move_to(last_gear.pos_servo2);
}

void loop() {
  btn_shift_down.read();
  btn_shift_up.read();
}