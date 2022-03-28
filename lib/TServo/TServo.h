#pragma once

#include "Arduino.h"
#include <Servo.h>

class TServo {
  int _offset = 0;
  bool _reversed = false;
  Servo _servo = Servo{};

public:
  TServo(uint8_t pin, int offset, bool reversed, int min, int max);

  void move_to(uint pos);
};
