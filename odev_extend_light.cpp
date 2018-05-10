/**
 * @file odev_extend_light.cpp
 * @author gongpan
 * @version 1.1
 * @date 2017-06-22
 * @brief 光线传感器驱动
 */
 
 #include "odev_extend_light.h"

namespace Odev {

OdevExtendLight::OdevExtendLight(PinName a0) : p1(a0) {}

uint16_t OdevExtendLight::get_light_state() {
  return p1.read_u16();
}
}
