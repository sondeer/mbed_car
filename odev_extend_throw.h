#ifndef __ODEV_EXTEND_THROW__
#define __ODEV_EXTEND_THROW__

#include "mbed.h"
#include "odev_extend.h"

namespace Odev {

class OdevExtendThrow {
 public:
  OdevExtendThrow(PinName pwm1, PinName pwm2);
  void set_throw(uint16_t freq, uint16_t count);
  void set_throw(uint8_t channel, uint16_t freq, uint16_t count);

 private:
  PwmOut p1;
  PwmOut p2;
};
}

#endif
