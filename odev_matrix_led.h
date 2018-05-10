#ifndef __ODEV_MATRIX_LED__
#define __ODEV_MATRIX_LED__

#include "mbed.h"

namespace Odev {
struct buffer {
  uint8_t index;
  uint8_t data[24];
};

class OdevMatrixLed {
 public:
  OdevMatrixLed(PinName sda, PinName scl);
  void init();
  void display();
  void clear();
  void setBuffer(uint8_t* buff);

 private:
  I2C pc;
  struct buffer display_buffer;

  uint32_t last_update_tick;
};
}

#endif
