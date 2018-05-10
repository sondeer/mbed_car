#ifndef __ODEV_ACTION__
#define __ODEV_ACTION__

#include "mbed.h"

namespace Odev {
struct odev_action {
  uint8_t sound_index_h;
  uint8_t sound_index_l;
  uint8_t motor_left;
  uint8_t motor_right;
  uint32_t delay;
  const uint8_t* led_buffer;
  uint8_t type_mask;
};

struct face_action {
  const uint8_t* led_buffer;
  uint32_t delay;
};

#define action_power_off   0
#define action_power_on    1
#define action_power_charge    2

class OdevAction {
 public:
  OdevAction();
  void update(uint8_t sonar_distance, uint8_t mode);
  void update_ticks();
  void power_on_action();
  void power_off_action();
  void power_low_action();
  void idle_action();
  void key_action();
  void set_idle_speed(uint8_t speed);
 
  void obs_action();
  void roam_obs_action();
  void roam_no_obs_action();
  void follow_obs_action();
  void follow_no_obs_action();
  void line_mode_action();
  void draw_mode_action();
  void charge_mode_action();

  void set_manual_face(const uint8_t* buff);
  void set_silent_mode(uint8_t);

  void active();
  void test();

  void add_set_led_matrix_callback(Callback<void(const uint8_t*)> func);
  void add_set_motor_speed_callback(Callback<void(int8_t, int8_t)> func);
  void add_set_sound_callback(Callback<void(uint8_t, uint8_t)> func);

 private:
  uint32_t prev_active_tick;
  uint32_t idle_action_repeat_tick;
  // uint32_t last_action_tick;
  uint32_t prev_obs_action_tick;
  uint32_t prev_update_tick;
  uint32_t action_delay_tick;
  uint32_t action_idle_tick;

  uint32_t prev_roam_obs_action_tick;
  uint32_t prev_roam_no_obs_action_tick;
  uint32_t prev_follow_obs_action_tick;
  uint32_t prev_follow_no_obs_action_tick;
  uint32_t prev_line_mode_action_tick;
  uint32_t prev_draw_mode_action_tick;

  uint8_t idle;
  uint8_t avoid_obs;
  uint8_t silent_mode;

  uint8_t face_manual[24];
 
  uint32_t avoid_obs_fliter_tick;
  uint8_t action_power_state;
  uint8_t key_action_speed;
  uint8_t key_action_state;
  
  Callback<void(uint8_t, uint8_t)> set_sound_callback;
  Callback<void(int8_t, int8_t)> set_motor_speed_callback;
  Callback<void(const uint8_t*)> set_led_matrix_callback;

  CircularBuffer<odev_action, 64, uint8_t> action_buffer;
  // CircularBuffer<uint16_t, 32, uint8_t> music_buffer;

  void add_all_action_type(const uint8_t* buff, int8_t motor_left, int8_t motor_right, uint8_t a, uint8_t b, uint32_t delay);

  void set_led_buff(const uint8_t* buff);
  void set_speed(int8_t, int8_t);
  void set_sound(uint8_t, uint8_t);

  void add_face_action(uint8_t index, uint32_t delay);
  void add_motor_action(int8_t index);
  void add_sound(uint8_t, uint8_t);
  void add_sound(uint8_t, uint8_t, uint32_t);
  void add_motor(int8_t, int8_t, uint32_t);

  void add_face(const uint8_t* buff, uint32_t delay);

  void clear_action_buffer_and_stop_motor();
  void update_mode_action_ticks();
  
  void set_eye_face(void);
  
};
}

#endif
