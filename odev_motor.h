#ifndef __ODEV_MOTOR_
#define __ODEV_MOTOR_

#include "mbed.h"

namespace Odev {
class OdevMotor {
 private:
  PwmOut pin_m1;
  PwmOut pin_m2;
 
	float target_speed, current_speed;
  uint32_t last_update_tick;

 public:
  OdevMotor(PinName m1, PinName m2);
  void setPwm(float speed);
  void update();
};
}

#endif
