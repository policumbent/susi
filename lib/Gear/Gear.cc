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
