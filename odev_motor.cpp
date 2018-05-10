#include "odev_motor.h"

namespace Odev {

OdevMotor::OdevMotor(PinName m1, PinName m2) : pin_m1(m1), pin_m2(m2) {
  //pin_m1.period_us(1000);
  //pin_m2.period_us(1000);
  pin_m1 = 0;
  pin_m2 = 0;
  target_speed = 0.0;
  current_speed = 0.0;
	
  //pin_m1.period_us(10000);
  //pin_m2.period_us(10000);
}

void OdevMotor::setPwm(float speed) {
  /*if (speed > 0) {
    pin_m1 = 0;
    pin_m2 = fabs(speed);
  } else if (speed < 0) {
    pin_m1 = fabs(speed);
    pin_m2 = 0;
  } else {
    pin_m1 = 0;
    pin_m2 = 0;
  } */

  target_speed = speed;
}

void OdevMotor::update() {
  //if(current_speed == target_speed) {
  //	return;
  //}

  if ((us_ticker_read() - last_update_tick) > 10000) {
    if (fabs(target_speed - current_speed) > 0.1) {
      if (target_speed > current_speed) {
        current_speed += 0.1;
      } else if (target_speed < current_speed) {
        current_speed -= 0.1;
      }
    } else {
      current_speed = target_speed;
    }

    if (current_speed > 0) {
      pin_m1 = 1-fabs(current_speed);
      pin_m2 = 1;
    } else if (current_speed < 0) {
      pin_m1 = 1;
      pin_m2 = 1-fabs(current_speed);
    } else {
      pin_m1 = 1;
      pin_m2 = 1;
    }

    last_update_tick = us_ticker_read();
  }
}
}
