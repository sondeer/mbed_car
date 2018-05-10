#ifndef __ODEV_ULTRASONIC_SENSOR__
#define __ODEV_ULTRASONIC_SENSOR__

#include "mbed.h"

namespace Odev {
enum ultrasonic_state {
  ULTRASONIC_IDLE,
  ULTRASONIC_TRIG,
  ULTRASONIC_WAIT_RISE,
};

class OdevUltrasonicSensor {
 public:
  OdevUltrasonicSensor(PinName power, PinName tx, PinName gain, PinName rx);
  // void trig();
  // void start_measure();
  void end_measure();
  void update();
  uint32_t get_distance();
  uint8_t triged;

 private:
  DigitalOut power_pin;
  DigitalOut trig_pin;
  DigitalInOut gain_pin;
  InterruptIn rx_pin;

  uint32_t distance;
  uint32_t start_measure_tick;
  uint32_t end_measure_tick;
  uint8_t measure_state;
};
}

#endif
