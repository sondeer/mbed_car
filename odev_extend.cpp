#include "odev_extend.h"

namespace Odev {
OdevExtend::OdevExtend(PinName tx, PinName rx, PinName pwm1, PinName pwm2) : pc(tx, rx), p1(pwm1), p2(pwm2) {}

void OdevExtend::set_pwm(uint16_t freq, uint16_t count) {
  float pwm_value;

  p1.period_us(1000000 / freq);
  p2.period_us(1000000 / freq);

  pwm_value = count;

  p1 = (freq * pwm_value) / 1000000.0f;
  p2 = (freq * pwm_value) / 1000000.0f;
}

void OdevExtend::set_pwm(uint8_t channel, uint16_t freq, uint16_t count){
}


}
