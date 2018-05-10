#ifndef __ODEV_EXTEND_TEMP__
#define __ODEV_EXTEND_TEMP__

#include "mbed.h"

namespace Odev {
class OdevExtendTemp {
 public:
  OdevExtendTemp(PinName pin_data);
  float get_temperature();
  float get_humidity();
  void update();

 private:
  uint32_t prev_update_tick;
  uint8_t sensor_data[5];

  float temperature;
  float humidity;

  DigitalInOut p1;
 
  uint8_t stall(uint8_t const level, uint16_t const max_time);
};
}
#endif
