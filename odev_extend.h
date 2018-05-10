#ifndef __ODEV_EXTEND__
#define __ODEV_EXTEND__

#include "mbed.h"

namespace Odev {
class OdevExtend {
 public:
  OdevExtend(PinName tx, PinName rx, PinName pwm1, PinName pwm2);
  void set_pwm(uint16_t freq, uint16_t count);
  void set_pwm(uint8_t channel, uint16_t freq, uint16_t count);

 private:
  RawSerial pc;
  PwmOut p1;
  PwmOut p2;
};
}

#endif
