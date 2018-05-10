#include "odev_extend_throw.h"

namespace Odev {

OdevExtendThrow::OdevExtendThrow(PinName pwm1, PinName pwm2) : p1(pwm1), p2(pwm2) {}

void OdevExtendThrow::set_throw(uint16_t freq, uint16_t count) {
  float pwm_value;

  p1.period_us(1000000 / freq);
  p2.period_us(1000000 / freq);

  pwm_value = count;

  p1 = (freq * pwm_value) / 1000000.0f;
  p2 = (freq * pwm_value) / 1000000.0f;
}
}
