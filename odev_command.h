#ifndef __ODEV_COMMAND__
#define __ODEV_COMMAND__

#include "mbed.h"

namespace Odev {

struct frame {
  uint8_t header;
  uint8_t serialNum;
  uint8_t cmdAndLength;
  uint8_t data[17];
};

class OdevCommand {
 public:
  OdevCommand(PinName tx, PinName rx);
  void update();
  uint8_t getAppActiveState(void);
  uint8_t getAppConnectState(void);

 private:
  RawSerial pc;
  CircularBuffer<uint8_t, 64, uint8_t> rx_buffer;
  void rx_irq();
  uint8_t state;
  struct frame rx_frame;
  // struct frame tx_frame;
  uint8_t data_left;
  void recvPacket();
  void recvExtPacket();
  void getState();
  void setState();
  void sendFrame(uint8_t cmd, uint8_t sNum, uint8_t* data, int length);
  uint8_t getCheckSum(void* buff, int size);
  uint8_t checkSum(void* buff, int size);
 
  void entry_test_mode(void);
  
  uint32_t last_command_update_tick;
  uint32_t pre_app_active_tick;
  uint32_t pre_app_connect_tick;
  uint8_t app_task_mode;
  uint8_t on_test_mode;
 
 private:
  Callback<void(int8_t, int8_t)> on_set_motor_speed;
  Callback<void(uint8_t)> on_set_rocket_speed;
  Callback<void(uint8_t, uint16_t, uint16_t)> on_set_throw;
  Callback<void(uint8_t, uint8_t)> on_set_sound;
  Callback<void(uint8_t)> on_set_silent_mode;
  Callback<void(int8_t, int8_t)> on_set_mode;
  Callback<void(uint8_t*, uint8_t)> on_set_matrix_left;
  Callback<void(uint8_t*, uint8_t)> on_set_matrix_right;

  Callback<void(uint16_t, uint16_t)> on_set_beep_freq;
  Callback<void(uint8_t, int16_t, uint8_t)> on_set_seg_display;

  Callback<void(void)> on_set_program_mode;
  Callback<void(void)> on_active;
 
  Callback<void(uint8_t)> on_set_aud_type;
  //Callback<void(uint8_t)> on_set_app_task_mode;

  Callback<uint8_t(void)> on_get_mode;
  Callback<uint8_t(void)> on_get_ultrasonic;
  Callback<uint8_t(void)> on_get_line;
  Callback<uint8_t(void)> on_get_battery;
  Callback<uint8_t(void)> on_get_key_state;
  Callback<uint8_t(void)> on_get_throw_state;

  Callback<void(uint8_t, int16_t*, int16_t*)> on_get_temperature;
  Callback<uint16_t(uint8_t)> on_get_light_sensor_state;
  Callback<uint16_t(uint8_t)> on_get_sound_sensor_state;
  Callback<uint16_t(uint8_t)> on_get_motion_sensor_state;
  Callback<uint16_t(uint8_t)> on_get_gas_sensor_state;
  Callback<uint16_t(uint8_t)> on_get_flame_sensor_state;
  
  Callback<uint16_t(void)> on_get_aud_type;
  
  Callback<uint16_t(void)> on_get_charge_mode;
  Callback<void(uint8_t)>  on_set_test_mode_speed;

  Callback<uint16_t(uint8_t)> on_get_line_sensor_value;
  Callback<uint16_t(uint8_t)> on_get_line_sensor_level;
  
  Callback<void(uint8_t,uint16_t)> on_set_line_sensor_value;
  Callback<void(uint16_t,uint16_t,uint16_t,uint16_t)> on_save_line_sensor_data;
  
  //uint32_t prev_update_tick;
	uint32_t watchdog_update_tick;
   
 public:
  void add_motor_speed_callback(Callback<void(int8_t, int8_t)> func) { on_set_motor_speed = func; };
  void add_rocket_speed_callback(Callback<void(uint8_t)> func) { on_set_rocket_speed = func; };
  void add_throw_callback(Callback<void(uint8_t, uint16_t, uint16_t)> func) { on_set_throw = func; };
  void add_sound_callback(Callback<void(uint8_t, uint8_t)> func) { on_set_sound = func; };
  void add_sound_off_callback(Callback<void(uint8_t)> func) { on_set_silent_mode = func; };
  void add_set_mode_callback(Callback<void(int8_t, int8_t)> func) { on_set_mode = func; };
  void add_set_program_mode_callback(Callback<void(void)> func) { on_set_program_mode = func; };
  void add_set_active_callback(Callback<void(void)> func) { on_active = func; };
  void add_set_aud_type_callback(Callback<void(uint8_t)> func) { on_set_aud_type = func; };
  //void add_set_app_task_mode_callback(Callback<void(uint8_t)> func) { on_set_app_task_mode = func; };
  
  void add_matrix_left_callback(Callback<void(uint8_t*, uint8_t)> func) { on_set_matrix_left = func; };
  void add_matrix_right_callback(Callback<void(uint8_t*, uint8_t)> func) { on_set_matrix_right = func; };
  void add_get_mode_callback(Callback<uint8_t(void)> func) { on_get_mode = func; };
  void add_get_ultrasonic_callback(Callback<uint8_t(void)> func) { on_get_ultrasonic = func; };
  void add_get_line_callback(Callback<uint8_t(void)> func) { on_get_line = func; };
  void add_get_battery_callback(Callback<uint8_t(void)> func) { on_get_battery = func; };
  void add_get_key_state_callback(Callback<uint8_t(void)> func) { on_get_key_state = func; };
  void add_get_throw_state_callback(Callback<uint8_t(void)> func) { on_get_throw_state = func; };

  void add_set_beep_freq_callback(Callback<void(uint16_t, uint16_t)> func) { on_set_beep_freq = func; };
  void add_set_seg_display_callback(Callback<void(uint8_t, int16_t, uint8_t)> func) { on_set_seg_display = func; };

  void add_get_temperature_callback(Callback<void(uint8_t, int16_t*, int16_t*)> func) { on_get_temperature = func; };
  void add_get_light_sensor_state_callback(Callback<uint16_t(uint8_t)> func) { on_get_light_sensor_state = func; };
  void add_get_sound_sensor_state_callback(Callback<uint16_t(uint8_t)> func) { on_get_sound_sensor_state = func; };
  void add_get_motion_sensor_state_callback(Callback<uint16_t(uint8_t)> func) { on_get_motion_sensor_state = func; };
  void add_get_gas_sensor_state_callback(Callback<uint16_t(uint8_t)> func) { on_get_gas_sensor_state = func; };
  void add_get_flame_sensor_state_callback(Callback<uint16_t(uint8_t)> func) { on_get_flame_sensor_state = func; };
  
  void add_get_aud_type_callback(Callback<uint16_t(void)> func) { on_get_aud_type = func; };
  void add_get_charge_mode_callback(Callback<uint16_t(void)> func) { on_get_charge_mode = func; };
  
  void add_set_test_mode_speed_callback(Callback<void(uint8_t)> func) { on_set_test_mode_speed = func; };
  
  void add_get_line_sensor_callback(Callback<uint16_t(uint8_t)> func){on_get_line_sensor_value=func;};  
  void add_get_line_sensor_level_callback(Callback<uint16_t(uint8_t)> func){on_get_line_sensor_level=func;};  
  
  void add_set_line_sensor_callback(Callback<void(uint8_t,uint16_t)> func){on_set_line_sensor_value=func;}
  
  void add_sv_line_sensor_data_callback(Callback<void(uint16_t,uint16_t,uint16_t,uint16_t)> func){on_save_line_sensor_data=func;}

  void setAppTaskMode(uint8_t mode) ;	//0--start 1--control,2--progarm,3--cloud,4--task

  uint8_t getAppTaskMode(void);
  
  void set_test_mode(uint8_t mode);
  uint8_t get_test_mode(void);
};
}

#endif
