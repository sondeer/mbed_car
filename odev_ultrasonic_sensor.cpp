#include "odev_ultrasonic_sensor.h"

namespace Odev {

OdevUltrasonicSensor::OdevUltrasonicSensor(PinName power, PinName tx, PinName gain, PinName rx) : power_pin(power), trig_pin(tx), gain_pin(gain, PIN_OUTPUT, OpenDrainNoPull, 1), rx_pin(rx) {
  power_pin = 1;
  rx_pin.rise(callback(this, &OdevUltrasonicSensor::end_measure));
  measure_state = ULTRASONIC_IDLE;
  start_measure_tick = us_ticker_read();
  end_measure_tick = start_measure_tick;
}

void OdevUltrasonicSensor::update() {
  switch (measure_state) {
    case ULTRASONIC_IDLE:
      if ((us_ticker_read() - end_measure_tick) > 100000) {
        measure_state = ULTRASONIC_TRIG;
      }

      break;
    case ULTRASONIC_TRIG:
			core_util_critical_section_enter();
      gain_pin = 0;
      for (int i = 0; i < 8; i++) {
        trig_pin = 1;
        wait_us(9);//(10);
        trig_pin = 0;
        wait_us(10);
      }
      gain_pin = 1;
			core_util_critical_section_exit();
      start_measure_tick = us_ticker_read();
      measure_state = ULTRASONIC_WAIT_RISE;

      break;

    case ULTRASONIC_WAIT_RISE:
      if ((us_ticker_read() - start_measure_tick) > 100000) {
        // end_measure_tick = us_ticker_read();
        distance = 100;
        measure_state = ULTRASONIC_IDLE;
      }

      if (triged) {
        distance = (end_measure_tick - start_measure_tick) * 0.034 / 2;
        if (distance > 100)
          distance = 100;
        measure_state = ULTRASONIC_IDLE;
        triged = 0;
      }

      break;

    default:
      break;
  }
}

void OdevUltrasonicSensor::end_measure() {
  if (measure_state == ULTRASONIC_WAIT_RISE) {
    end_measure_tick = us_ticker_read();
    triged = 1;
  } else {
    triged = 0;
  }
}

uint32_t OdevUltrasonicSensor::get_distance() {
  return distance;
}
}
