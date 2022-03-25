#include <Arduino.h>
#include <BfButton.h>
#include <ESP32Servo.h>

#include "Calibration.h"
#include "Gear.h"

const uint8_t pin_servo1 = 26;
const uint8_t pin_servo2 = 27;
const uint8_t pin_shift_up_btn = 12;
const uint8_t pin_shift_down_btn = 14;

BfButton shift_up_btn(BfButton::STANDALONE_DIGITAL, pin_shift_up_btn, true, HIGH);
BfButton shift_down_btn(BfButton::STANDALONE_DIGITAL, pin_shift_down_btn, true, HIGH);

auto vec_servo1 = GearVec{};
auto vec_servo2 = GearVec{};
auto cal = Calibration{&vec_servo1, &vec_servo2};
auto servo1 = Servo{};
auto servo2 = Servo{};

uint8_t current_gear_id = 0;

void shift_up(BfButton *btn, BfButton::press_pattern_t pattern) {
  switch (pattern) {
  case BfButton::SINGLE_PRESS:
    Serial.println(" single pressed.");
    if (current_gear_id < MAX_GEAR) {
      auto gear = ++current_gear_id;
      auto pos1 = vec_servo1[gear - 1].shift_up();
      auto pos2 = vec_servo2[gear - 1].shift_up();

      servo1.write(static_cast<int>(pos1));
      servo2.write(static_cast<int>(pos2));

      Memory::save_gear(MemGear{gear, pos1, pos2});
    }
    break;
    // activate calibration
  case BfButton::LONG_PRESS:
    cal.isActive() ? cal.end() : cal.begin();
    Serial.println(" long pressed.");
    break;
  default:
    break;
  }
}

void shift_down(BfButton *btn, BfButton::press_pattern_t pattern) {
  if (pattern == BfButton::SINGLE_PRESS) {
    Serial.println(" single pressed.");

    if (current_gear_id > MIN_GEAR) {
      auto gear = --current_gear_id;
      auto pos1 = vec_servo1[gear - 1].shift_down();
      auto pos2 = vec_servo2[gear - 1].shift_down();

      servo1.write(static_cast<int>(pos1));
      servo2.write(static_cast<int>(pos2));

      Memory::save_gear(MemGear{gear, pos1, pos2});
    }
  }
}

void setup() {
  delay(500); // debouncing

  Serial.begin(115200);

  // button logic
  shift_up_btn.onPress(shift_up).onPressFor(shift_up, 2000);
  shift_up_btn.onPress(shift_down);

  // retrive configuration
  vec_servo1 = Memory::load_config_servo1();
  vec_servo2 = Memory::load_config_servo2();

  // servo configuration
  servo1.attach(pin_servo1, 700, 2191);
  servo2.attach(pin_servo2, 800, 2200);

  // retrive last gear
  auto last_gear = Memory::load_gear();
  current_gear_id = last_gear.id;

  // TODO: verify if it's useful
  servo1.write(static_cast<int>(last_gear.pos_servo1));
  servo2.write(static_cast<int>(last_gear.pos_servo2));
}

void loop() {
  shift_down_btn.read();
  shift_up_btn.read();
}