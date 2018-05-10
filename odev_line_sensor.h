#ifndef __ODEV_LINE_SENSOR_
#define __ODEV_LINE_SENSOR_

#include "mbed.h"

namespace Odev {

class OdevLineSensor {
 public:
  OdevLineSensor(PinName p0, PinName p1, PinName p2, PinName p3);
  uint8_t getState();
  int8_t getError();
  void update();
  uint16_t getLineSensorValue(int8_t senNum);
  void setLineSensorValue(int8_t senNum,uint16_t senValue);
 
  uint16_t getLineSensorLevel(int8_t senNum);
   

 private:
  AnalogIn a0;
  AnalogIn a1;
  AnalogIn a2;
  AnalogIn a3;

  uint16_t trig_a, trig_b, trig_c, trig_d;
  uint16_t adc_value_a, adc_value_b, adc_value_c, adc_value_d;

  uint8_t sensors_state;

  uint32_t last_measure_tick;
};
}

#endif
