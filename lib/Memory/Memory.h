#pragma once

#include "Gear.h"
#include <Arduino.h>
#include <Preferences.h>

/**
 * Gear information saved on memory
 *
 * @param id number of the gear
 * @param pos_servo1 relative position of the motor1
 * @param pos_servo2 relative position of the motor2
 */
struct MemGear {
  uint8_t id;
  uint pos_servo1;
  uint pos_servo2;
};

/**
 * Abstraction layer for flash memory operations
 */
namespace Memory {
void save_gear(const MemGear &value);
MemGear load_gear();

void save_config(const GearVec &servo1, const GearVec &servo2);
GearVec load_config_servo1();
GearVec load_config_servo2();
} // namespace Memory
