#ifndef __ODEV_SOUND__
#define __ODEV_SOUND__

#include "mbed.h"

namespace Odev {
class OdevSound {
 public:
  OdevSound(PinName tx, PinName rx);
  uint8_t get_aud_type(void);
  void play(uint8_t type, uint8_t index);
  void setVolume(uint8_t);
  void init();
  void update();
  void stop();

 private:
  RawSerial pc;
  void send(uint8_t* buff, uint8_t len);
  uint8_t getSum(uint8_t* buff, uint8_t len);
  uint8_t state;
  uint32_t prev_update_tick;
  
};
}

#endif
