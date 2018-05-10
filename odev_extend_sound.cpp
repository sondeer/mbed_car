#include "odev_extend_sound.h"

namespace Odev {

OdevExtendSound::OdevExtendSound(PinName a0) : p1(a0) {}

uint16_t OdevExtendSound::get_sound_state() {
  return p1.read_u16();
}
}
