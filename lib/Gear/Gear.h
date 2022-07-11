#pragma once

#include <Arduino.h>
#include <vector>

const uint8_t MIN_GEAR = 1;

/**
 * Define the relative position of the servo
 *
 * @param pos_up indicate the low->high gear change (es. from 1^ gear to 2^)
 * @param pos_down indicate the high->low gear change (es. from 2^ gear to 1^)
 */
struct Position {
  uint up;
  uint down;
};

/**
 * Define the single gear with all its parameters
 *
 * @param id Number of the gear
 * @param pos Positions for the gear change
 */
class Gear {
public:
  uint8_t id = 0;
  Position pos = {0, 0};

  Gear() = default;
  Gear(uint i, Position pos) : id(i), pos(pos){};

  uint shift_up() const;
  uint shift_down() const;
};

/**
 * Define a vector of gear
 */
class GearVec : public std::vector<Gear>{
public:
  GearVec(): std::vector<Gear>(MAX_GEAR){};
  ~GearVec()=default;

  /**
   * Fill `id` field of the `Gear` object in `id-1` vec position
   */
  void fill();
};