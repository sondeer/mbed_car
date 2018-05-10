#include "odev_extend_temp.h"

namespace Odev {

OdevExtendTemp::OdevExtendTemp(PinName pin_data) : p1(pin_data) {
  prev_update_tick = us_ticker_read();

  temperature = 0.0;
  humidity = 0.0;
}

float OdevExtendTemp::get_temperature() {
  return temperature;
}

float OdevExtendTemp::get_humidity() {
  return humidity;
}

uint8_t OdevExtendTemp::stall(uint8_t const level, uint16_t const max_time) {
  uint16_t cnt = 0;
  while (level == p1) {
    if (cnt > max_time) {
      return 1;
    }
    cnt++;
    wait_us(1);
  }
  return 0;
}

void OdevExtendTemp::update() {
  uint8_t i, j, temp_value;
  uint8_t bit_value[40];
  uint8_t checksum;

  //if ((us_ticker_read() - prev_update_tick) > 500000) {
    prev_update_tick = us_ticker_read();

    p1.output();
    p1 = 0;
    wait_ms(18);
    p1 = 1;
    wait_us(30);

    p1.input();

    if (0 != stall(1, 100)) {
      return;
    }
    // sensor should signal low 80us and then hi 80us
    if (0 != stall(0, 100)) {
      return;
    }
    if (0 != stall(1, 100)) {
      return;
    }

    for (i = 0; i < 5; i++) {
      for (j = 0; j < 8; j++) {
        if (0 != stall(0, 75)) {
          return;
        }
        wait_us(40);
        bit_value[i * 8 + j] = p1;
        if (0 != stall(1, 50)) {
          return;
        }
      }
    }

    for (i = 0; i < 5; i++) {
      temp_value = 0;
      for (j = 0; j < 8; j++) {
        if (bit_value[i * 8 + j] == 1) {
          temp_value |= (1 << (7 - j));
        }
      }
      sensor_data[i] = temp_value;
    }

    checksum = sensor_data[0] + sensor_data[1] + sensor_data[2] + sensor_data[3];

    if (checksum == sensor_data[4]) {
      humidity = sensor_data[0];
      temperature = sensor_data[2];
    }
  //}
}
}
