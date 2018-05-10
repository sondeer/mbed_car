#ifndef __ODEV_MOTION__
#define __ODEV_MOTION__

#include "mbed.h"

namespace Odev {
class OdevMotionControl {
 public:
  OdevMotionControl();
  void update(uint8_t line, uint8_t sonar);
  void set_wheel_speed(float, float);
  void set_rocket_speed(float);
  void set_mode(uint8_t);
  void add_set_pwn_callback(Callback<void(float, float)> func) { set_pwm = func; };
  uint8_t get_mode();
  void active();
  
  void add_get_test_mode_callback(Callback<uint8_t(void)> func) { get_test_mode = func; };
  void set_test_mode_speed(float speed);
  
 private:
  void set_velocity(float, float);

 private:
  uint8_t motion_mode;
  uint32_t last_update_tick;
  uint32_t prev_roam_back_tick;
  uint32_t prev_roam_turn_tick;
  uint32_t prev_active_tick;
  uint32_t test_mode_tick;

  float speed_left, speed_right, rocket_speed;

  int8_t roam_mode_random_dir;
  int8_t line_mode_last_turn_dir;
 
  float prev_line_sensor_error;
  float prev_line_angluar_speed;

  enum {
    ROAM_NORMAL,
    ROAM_RANDOM_TURN,
    ROAM_GO_BACK,
  } roam_state;

  Callback<void(float, float)> set_pwm;

  void normal_mode_process(uint8_t line, uint8_t sonar);
  void roam_mode_process(uint8_t line, uint8_t sonar);
  void line_mode_process(uint8_t line, uint8_t sonar);
  void follow_mode_process(uint8_t line, uint8_t sonar);
  
  void test_mode_process(void);
  
  Callback<uint8_t(void)> get_test_mode;

  uint8_t has_teltop_velocity();

  int8_t getLineSensorError(uint8_t line);
};
}

#endif
