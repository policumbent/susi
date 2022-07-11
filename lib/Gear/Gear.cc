#include "Gear.h"

/**
 * Retrive low->high gear position
 *
 * @return position
 */
uint Gear::shift_up() const { return this->pos.up; }

/**
 * Retrive high->low gear position
 *
 * @return position
 */
uint Gear::shift_down() const { return this->pos.down; }

void GearVec::fill() {
  for (uint i=0; i<MAX_GEAR; i++){
    (*this)[i].id = i+1;
  }
}