#include "odev_power_manage.h"
#include "odev_myticker.h"

namespace Odev {
//OdevPowerManage::OdevPowerManage(PinName key, PinName ctrl, PinName adc, PinName v5) : pin_ctrl(ctrl), pin_key(key), pin_bat_adc(adc), pin_5v_ctrl(v5) {
OdevPowerManage::OdevPowerManage(PinName key, PinName ctrl, PinName adc, PinName v5,PinName pwr_charge)\ 
	: pin_ctrl(ctrl), pin_key(key), pin_bat_adc(adc), pin_5v_ctrl(v5) ,pin_charge(pwr_charge,PullUp) {
  p_state = POWER_INIT;
  pin_ctrl = 1;
  pin_5v_ctrl = 1;
	
  key_filter = 0;
  prev_key_off_tick = my_ticker_read();
  prev_key_on_tick = my_ticker_read();
  key_query_state = 0;
  key_pressed = 0;
}

uint8_t OdevPowerManage::getKeyState() {
  if (key_query_state) {
    key_query_state = 0;
    return 1;
  }
  return 0;
}

uint8_t OdevPowerManage::getPowerState() {
  if (p_state == POWER_WAIT_OFF || p_state == POWER_OFF) {
    return 1;
  } else {
    return 0;
  }
}

uint8_t OdevPowerManage::getCurrentPowerState() {
   return p_state;
}

void OdevPowerManage::set_power_on() {
  p_state = POWER_WAIT_OFF;
  pin_ctrl = 1;
  if (on_func) {
    on_func();
  }
}

void OdevPowerManage::loop() {
	
  static uint32_t getTickTimeCount = 0;

  power_key_state = !pin_key;
	
  power_charge_state = pin_charge;

  if (power_key_state) {
      prev_key_off_tick = my_ticker_read();
	  getTickTimeCount=0;
  } else {
	  prev_key_on_tick = my_ticker_read();  
	  getTickTimeCount++;
  }

  if ((my_ticker_read() - prev_key_off_tick) > 10)
  {
    key_query_state = 1;
  }

  if ((my_ticker_read() - prev_key_on_tick) > 100)
  {
    key_query_state = 0;
  }

  switch (p_state) {
    case POWER_INIT:
	  if ((my_ticker_read() - prev_key_off_tick) > 80)
	  {
		p_state = POWER_ON;
		if (on_func) {
		  on_func();
		}
	  }

      if ((my_ticker_read() - prev_key_on_tick) > 1) 
	  {
        pin_ctrl = 0;
      }
      //检测到充电时，进入充电模式
      if(power_charge_state == 0 &&(my_ticker_read() - prev_key_on_tick) > 100) 
	  {
		 p_state = POWER_CHARGE;
		 if (power_charge_func) {
          power_charge_func();
         }
	  }
      break;

    case POWER_ON:
      pin_ctrl = 1;
      if (!previou_power_key_state && power_key_state) {
        p_state = POWER_WAIT_OFF;
      }
      break;

    case POWER_WAIT_OFF:
	  if( ((my_ticker_read() - prev_key_off_tick) > 100)
		  &&(getTickTimeCount > 5000))
	  {
        p_state = POWER_OFF;
        if (off_func) {
          off_func();
        }
		getTickTimeCount = 0;	
      } 
	  else if (key_pressed == 0 && (my_ticker_read() - prev_key_off_tick) > 10)//100000) 
	  {
			key_pressed = 1;
	  }
			
	 if(key_pressed == 1 &&(my_ticker_read() - prev_key_on_tick) > 10)//100000) 
	 {
		if(key_press_func) {
			key_press_func();
		}
		key_pressed = 0;
	  }
	  //检测到充电时，进入充电模式
      if(power_charge_state == 0 )//&&(my_ticker_read() - prev_key_on_tick) > 10)//100000) 
	  {
		 p_state = POWER_CHARGE;
		 if (power_charge_func) {
          power_charge_func();
         }
	  }
      break;

    case POWER_OFF:
      if ((my_ticker_read() - prev_key_on_tick) > 150) 
	  {
        pin_ctrl = 0;
		//清除状态避免接USB开关机出问题
		prev_key_on_tick = my_ticker_read();
		p_state = POWER_INIT;  
      }
      break;
	case POWER_CHARGE:	//充电状态
	  //充电状态本身相当于关机状态
      //如果案件关机，因USB供电会重启	
//	  if (key_pressed == 0 && (my_ticker_read() - prev_key_off_tick) > 10)//100000) 
//	  {
//		 key_pressed = 1;
//	  }
//			
//	  if(key_pressed == 1 &&(my_ticker_read() - prev_key_on_tick) > 10)//100000) 
//	  {
//		if(key_press_func) {
//			key_press_func();
//		}
//		key_pressed = 0;
//	  }
	  //检测USB断开连接时或者充满时，进入关机状态

//由于认证要求，充电不能玩耍，连表情都不需要
      if( power_charge_state == 1 )//&&(my_ticker_read() - prev_key_on_tick) > 10)//100000) 
	  {
		 p_state = POWER_INIT;
		 //pin_ctrl = 0;
	  }
	  break;
    default:
      break;
  }
  previou_power_key_state = power_key_state;
}

void OdevPowerManage::add_power_on_callback(Callback<void()> func) {
  on_func = func;
}

void OdevPowerManage::add_power_off_callback(Callback<void()> func) {
  off_func = func;
}

void OdevPowerManage::add_key_press_callback(Callback<void()> func) {
  key_press_func = func;
}

void OdevPowerManage::add_power_charge_callback(Callback<void()> func) {
  power_charge_func = func;
}

void OdevPowerManage::power_on() {
  pin_ctrl = 1;
}

void OdevPowerManage::set_power_state(uint8_t state) {	
	p_state = state;
}

float OdevPowerManage::get_bat_level() {
  return pin_bat_adc;
}
}
