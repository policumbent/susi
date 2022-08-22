#include "TServo.h"

TServo::TServo(uint8_t pin, int offset, bool reversed, int minPulseWidth, int maxPulseWidth):_offset(offset),_reversed(reversed) {
  _servo.attach(pin, Servo::CHANNEL_NOT_ATTACHED, 0, 180, minPulseWidth, maxPulseWidth);
}

void TServo::move_to(uint pos) {
  auto position = static_cast<int>(_offset+pos);

  // check position value
  if (position > 180){
    position = 180;
  }else if(position < 0){
    position = 0;
  }

  if(_reversed){
    _servo.write(180 - position);
  }else{
    _servo.write(position);
  }
}
