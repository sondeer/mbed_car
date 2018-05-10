#ifndef __ODEV_POWER_MANAGE__
#define __ODEV_POWER_MANAGE__

#include "mbed.h"

namespace Odev {

enum power_state {
  POWER_INIT,
  POWER_ON,
  POWER_WAIT_OFF,
  POWER_OFF,
  POWER_CHARGE,
};

class OdevPowerManage {
 private:
  DigitalOut pin_ctrl;
  DigitalIn pin_key;
  AnalogIn pin_bat_adc;
  DigitalOut pin_5v_ctrl;
  DigitalIn pin_charge;
 
  uint32_t prev_key_off_tick;
  uint32_t prev_key_on_tick;

  uint32_t key_filter;
  uint8_t key_query_state;
  uint8_t key_pressed;
  // bool key_state;
  bool power_key_state, previou_power_key_state;
  uint8_t p_state;
  float bat_level;
 
  uint8_t power_charge_state;

  Callback<void()> on_func;
  Callback<void()> off_func;
  Callback<void()> key_press_func;
  // Callback<void()> low_func;
  Callback<void()> power_charge_func;
  
 public:
  OdevPowerManage(PinName key, PinName ctrl, PinName adc, PinName v5,PinName pwr_charge);
  uint8_t getKeyState();
  uint8_t getPowerState();
  uint8_t getCurrentPowerState();
  void loop();
  void add_power_on_callback(Callback<void()> func);
  void add_power_off_callback(Callback<void()> func);
  void add_key_press_callback(Callback<void()> func);
  // void add_power_low_callback(Callback<void()> func);
  void add_power_charge_callback(Callback<void()> func);
 
  void power_on();
  void set_power_on();
  void set_power_state(uint8_t state);
 
  float get_bat_level();
};
}
#endif
