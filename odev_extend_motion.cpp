#include "odev_extend_motion.h"

namespace Odev {

OdevExtendMotion::OdevExtendMotion(PinName d, PinName pin_mode) : p1(d), p2(pin_mode) {}

uint8_t OdevExtendMotion::get_motion_state() {
  return p1;
}
}
