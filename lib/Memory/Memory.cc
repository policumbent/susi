#include "Memory.h"
#include <mutex>

Preferences prefs;

/**
 * This namespace is accessed from the `Calibration` server with an async thread
 * and from the main loop for the feedback value save, so a kind of
 * synchronization is needed, because `Preferences` can be accessed one by one
 */
std::mutex m;

/**
 * Save last used gear for the next startup
 *
 * @param value gear to save
 */
void Memory::save_gear(const MemGear &value) {
  std::lock_guard<std::mutex> l(m);

  prefs.begin("last_gear", false);
  prefs.putBytes("last_gear", &value, sizeof(value));
  prefs.end();
}

/**
 * Retrieve last used gear
 *
 * On startup this is the current configuration of the physical gear
 * @return last used gear
 */
MemGear Memory::load_gear() {
  std::lock_guard<std::mutex> l(m);

  auto last = MemGear{};

  prefs.begin("last_gear", false);
  if (prefs.isKey("last_gear")) {
    prefs.getBytes("last_gear", &last, sizeof(last));
  }
  prefs.end();

  return last;
}

/**
 * Save configuration after calibration
 *
 * @param updated configuration
 */
void Memory::save_config(const GearVec &servo1, const GearVec &servo2) {
  std::lock_guard<std::mutex> l(m);

  prefs.begin("config_servo1", false);
  prefs.putBytes("config_servo1", servo1.data(), servo1.size() * sizeof(Gear));
  prefs.end();

  prefs.begin("config_servo2", false);
  prefs.putBytes("config_servo2", servo2.data(), servo2.size() * sizeof(Gear));
  prefs.end();
}

/**
 * Retrieve gears configuration
 *
 * @return configuration of motor1
 */
GearVec Memory::load_config_servo1() {
  std::lock_guard<std::mutex> l(m);

  auto vec = GearVec{};

  vec.fill();

  prefs.begin("config_servo1", false);
  if (prefs.isKey("config_servo1") &&
      prefs.getBytesLength("config_servo1") == vec.size() * sizeof(Gear)) {
    prefs.getBytes("config_servo1", vec.data(), vec.size() * sizeof(Gear));
  }
  prefs.end();

  return vec;
}
/**
 * Retrieve gears configuration
 *
 * @return configuration of motor2
 */

GearVec Memory::load_config_servo2() {
  std::lock_guard<std::mutex> l(m);

  auto vec = GearVec{};

  vec.fill();

  prefs.begin("config_servo2", false);
  if (prefs.isKey("config_servo2") &&
      prefs.getBytesLength("config_servo2") == vec.size() * sizeof(Gear)) {
    prefs.getBytes("config_servo2", vec.data(), vec.size() * sizeof(Gear));
  }
  prefs.end();

  return vec;
}
