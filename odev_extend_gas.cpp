/**
 * @file odev_extend_gas.cpp
 * @author gongpan
 * @version 1.1
 * @date 2017-06-22
 * @brief 气体传感器驱动
 */

#include "odev_extend_gas.h"

namespace Odev {

OdevExtendGas::OdevExtendGas(PinName d, PinName pin_mode) : p1(d), p2(pin_mode) {}

uint8_t OdevExtendGas::get_gas_state() {
  return p1;
}

uint16_t OdevExtendGas::get_gas_value() {
  return p2.read_u16();
}
}
