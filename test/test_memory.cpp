#include <Arduino.h>
#include <unity.h>

#include "Memory.h"
#include "Gear.h"

void test_gear_vec() {
  auto vec = GearVec{};

  vec.fill();

  for (uint i = 0; i < vec.size(); i++) {
    TEST_ASSERT_EQUAL(vec[i].id, i + 1);
  }
}

void test_save_gear() {
  auto gear = MemGear{1, 155, 255};
  Memory::save_gear(gear);

  auto saved = Memory::load_gear();

  TEST_ASSERT_EQUAL(gear.id, saved.id);
  TEST_ASSERT_EQUAL(gear.pos_servo1, saved.pos_servo1);
  TEST_ASSERT_EQUAL(gear.pos_servo2, saved.pos_servo2);
  TEST_ASSERT_EQUAL_MEMORY(&gear, &saved, sizeof(MemGear));
}

void test_save_config() {
  auto vec1 = GearVec{};
  for (uint i = 0; i < MAX_GEAR; i++) {
    vec1[i] = Gear{i + 1, {(i + 1) * 10, (i + 1) * 12}};
  }
  auto vec2 = GearVec{};
  for (uint i = 0; i < MAX_GEAR; i++) {
    vec2[i] = Gear{i + 1, {(i + 1) * 20, (i + 1) * 22}};
  }

  Memory::save_config(vec1, vec2);

  auto saved_vec1 = Memory::load_config_servo1();
  auto saved_vec2 = Memory::load_config_servo2();

  for (uint i = 0; i < MAX_GEAR; i++) {
    auto servo1 = vec1[i];
    auto saved_servo1 = saved_vec1[i];

    TEST_ASSERT_EQUAL(servo1.id, saved_servo1.id);
    TEST_ASSERT_EQUAL_MEMORY(&saved_servo1.pos, &servo1.pos, sizeof(Position));
    TEST_ASSERT_EQUAL_MEMORY(&saved_servo1, &servo1, sizeof(Gear));

    auto servo2 = vec2[i];
    auto saved_servo2 = saved_vec2[i];

    TEST_ASSERT_EQUAL(servo2.id, saved_servo2.id);
    TEST_ASSERT_EQUAL_MEMORY(&saved_servo2.pos, &servo2.pos, sizeof(Position));
    TEST_ASSERT_EQUAL_MEMORY(&saved_servo2, &servo2, sizeof(Gear));
  }

  TEST_ASSERT_EQUAL_MEMORY(vec1.data(), saved_vec1.data(),
                           vec1.size() * sizeof(Gear));
  TEST_ASSERT_EQUAL_MEMORY(vec2.data(), saved_vec2.data(),
                           vec2.size() * sizeof(Gear));
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_gear_vec);
  RUN_TEST(test_save_gear);
  RUN_TEST(test_save_config);
  UNITY_END();
}

// this is unused but needs for compilation
void loop() {}