/**
 * @file odev_extend_flame.cpp
 * @author gongpan
 * @version 1.1
 * @date 2017-06-22
 * @brief 火焰传感器驱动
 */

#include "odev_extend_flame.h"

namespace Odev {

OdevExtendFlame::OdevExtendFlame(PinName d, PinName pin_mode) : p1(d), p2(pin_mode) {}

uint8_t OdevExtendFlame::get_flame_state() {
  return p1;
}

uint16_t OdevExtendFlame::get_flame_value() {
  return p2.read_u16();
}
}
