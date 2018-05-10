#ifndef __ODEV_BUZZER__
#define __ODEV_BUZZER__

#include "mbed.h"

namespace Odev {
class OdevBuzzer {
 public:
  OdevBuzzer(PinName);
  void beep(uint32_t freq, uint32_t dut);
  void update();

 private:
  PwmOut p1;
  uint32_t start_beep_tick;
  uint32_t duration;
};
}
#endif
