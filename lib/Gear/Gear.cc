#include "Gear.h"

uint Gear::shift_up() const { return static_cast<int>(this->pos.up); }

uint Gear::shift_down() const { return static_cast<int>(this->pos.down); }
