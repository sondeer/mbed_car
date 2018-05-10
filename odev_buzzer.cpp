/**
 * @file odev_buzzer.cpp
 * @author gongpan
 * @version 1.1
 * @date 2017-06-22
 * @brief 蜂鸣器驱动
 */

#include "odev_buzzer.h"

namespace Odev {
OdevBuzzer::OdevBuzzer(PinName p) : p1(p) {
  start_beep_tick = us_ticker_read();
  duration = 0;
}

void OdevBuzzer::beep(uint32_t freq, uint32_t dut) {
  p1.period_us(1000000 / freq);
  p1 = 0.5;
  start_beep_tick = us_ticker_read();
  duration = dut;
}

void OdevBuzzer::update() {
  if ((us_ticker_read() - start_beep_tick) > duration) {
    p1 = 0.0;
    duration = 0;
  }
}
}
