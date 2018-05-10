#ifndef __ODEV_EXTEND_LIGHT__
#define __ODEV_EXTEND_LIGHT__

#include "mbed.h"

namespace Odev {
class OdevExtendLight {
 public:
  OdevExtendLight(PinName);
  uint16_t get_light_state();

 private:
  AnalogIn p1;
};
}
#endif
