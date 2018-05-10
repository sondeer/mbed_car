#ifndef __ODEV_EXTEND_MOTION__
#define __ODEV_EXTEND_MOTION__

#include "mbed.h"

namespace Odev {
class OdevExtendMotion {
 public:
  OdevExtendMotion(PinName, PinName);
  uint8_t get_motion_state();

 private:
  DigitalIn p1;
  DigitalOut p2;
};
}
#endif
