#include <Arduino.h>
#include <BfButton.h>
#include <ESP32Servo.h>

#include "Calibration.h"
#include "Gear.h"

const uint8_t pin_servo1 = 26;
const uint8_t pin_servo2 = 27;

BfButton btn_shift_up(BfButton::STANDALONE_DIGITAL, 12, true, FALLING);
BfButton btn_shift_down(BfButton::STANDALONE_DIGITAL, 14, true, FALLING);

auto vec_servo1 = GearVec{MAX_GEAR};
auto vec_servo2 = GearVec{MAX_GEAR};
auto cal = Calibration{&vec_servo1, &vec_servo2};
auto servo1 = Servo{};
auto servo2 = Servo{};

uint8_t current_gear_id = 0;

void btn_callback(BfButton *btn, BfButton::press_pattern_t pattern) {
  if (btn->getID() == btn_shift_down.getID() &&
      pattern == BfButton::SINGLE_PRESS && current_gear_id > MIN_GEAR) {

    auto id = --current_gear_id;
    auto pos1 = vec_servo1[id - 1].shift_down();
    auto pos2 = vec_servo2[id - 1].shift_down();

    servo1.write(static_cast<int>(pos1));
    servo2.write(static_cast<int>(pos2));
    Memory::save_gear(MemGear{id, pos1, pos2});
  } else if (btn->getID() == btn_shift_up.getID() &&
             pattern == BfButton::SINGLE_PRESS && current_gear_id < MAX_GEAR) {

    auto id = ++current_gear_id;
    auto pos1 = vec_servo1[id - 1].shift_up();
    auto pos2 = vec_servo2[id - 1].shift_up();

    servo1.write(static_cast<int>(pos1));
    servo2.write(static_cast<int>(pos2));
    Memory::save_gear(MemGear{id, pos1, pos2});
  } else if (btn->getID() == btn_shift_up.getID() &&
             pattern == BfButton::LONG_PRESS) {
    cal.isActive() ? cal.end() : cal.begin();
  }
}

void setup() {
  delay(500); // debouncing

  Serial.begin(115200);

  // button logic
  btn_shift_up.onPress(btn_callback).onPressFor(btn_callback, 2000);
  btn_shift_down.onPress(btn_callback);

  // retrive configuration
  vec_servo1 = Memory::load_config_servo1();
  vec_servo2 = Memory::load_config_servo2();

  // servo configuration
  servo1.attach(pin_servo1, 700, 2191);
  servo2.attach(pin_servo2, 800, 2200);

  // retrive last gear
  auto last_gear = Memory::load_gear();
  current_gear_id = last_gear.id;

  // retrive last position of the servo
  // TODO: verify if it's useful
  servo1.write(static_cast<int>(last_gear.pos_servo1));
  servo2.write(static_cast<int>(last_gear.pos_servo2));
}

void loop() {
  btn_shift_down.read();
  btn_shift_up.read();
}