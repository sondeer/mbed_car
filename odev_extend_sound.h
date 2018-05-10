#ifndef __ODEV_EXTEND_SOUND__
#define __ODEV_EXTEND_SOUND__

#include "mbed.h"

namespace Odev {
class OdevExtendSound {
 public:
  OdevExtendSound(PinName);
  uint16_t get_sound_state();

 private:
  AnalogIn p1;
};
}
#endif
