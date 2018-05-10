#ifndef __ODEV_EXTEND_FLAME__
#define __ODEV_EXTEND_FLAME__

#include "mbed.h"

namespace Odev {
class OdevExtendFlame {
 public:
  OdevExtendFlame(PinName, PinName);
  uint8_t get_flame_state();
  uint16_t get_flame_value();

 private:
  DigitalIn p1;
  AnalogIn p2;
};
}
#endif
