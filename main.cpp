/**
 * @file main.cpp
 * @author gongpan
 * @version 1.1
 * @date 2017-06-22
 * @brief 主程序
 */

#include "mbed.h"
#include "odev_power_manage.h"
#include "odev_command.h"
#include "odev_sound.h"
#include "odev_motor.h"
#include "odev_matrix_led.h"
#include "odev_line_sensor.h"
#include "odev_ultrasonic_sensor.h"
#include "odev_extend.h"
#include "odev_motion.h"
#include "odev_action.h"
#include "odev_extend_throw.h"
#include "odev_extend_temp.h"
#include "odev_extend_light.h"
#include "odev_extend_sound.h"
#include "odev_extend_motion.h"
#include "odev_extend_flame.h"
#include "odev_extend_gas.h"
#include "odev_extend_7seg_display.h"
#include "odev_buzzer.h"
#include "eeprom.h"
#include "odev_myticker.h"

#define POWER_STATE 0x200000C4

using namespace mbed;
using namespace Odev;

const uint32_t hardware_version __attribute__((at(0x80040C4))) = 0x001;
const uint32_t software_version __attribute__((at(0x80040C0))) = 0x002;

//#define AUD_CN_MODE		0x00
//#define AUD_EN_MODE     0x01
//uint32_t AudMode __attribute__((at(0x0801F800))) = 0x01;//默认的语音为英文

OdevPowerManage power_manager(PB_14, PB_15, PA_5, PB_12 ,PB_13);
OdevCommand command(PA_9, PA_10);
OdevSound sound(PC_10, PC_11);
OdevMotor motor_right(PC_7, PC_6);
OdevMotor motor_left(PC_8, PC_9);
OdevMatrixLed led(PB_9, PB_8);
OdevLineSensor line(PC_0, PC_1, PC_2, PC_3);
OdevUltrasonicSensor sonic(PB_2, PB_10, PB_11, PA_4);
// OdevExtend extend_port_a(PA_2, PA_3, PA_6, PA_7);
// OdevExtend extend_port_b(PC_4, PC_5, PB_0, PB_1);
OdevMotionControl motion;
OdevAction action;

OdevBuzzer buzzer(PA_1);
OdevExtendThrow* p_odev_throw = NULL;
OdevExtendTemp* p_temperature = NULL;
OdevExtendLight* p_light_sensor = NULL;
OdevExtendSound* p_sound_sensor = NULL;
OdevExtendMotion* p_motion_sensor = NULL;
OdevExtendFlame* p_flame_sensor = NULL;
OdevExtendGas* p_gas_sensor = NULL;
Odev7SegDisplay* p_seg_display = NULL;

float motor_speed[2];
float rocket_speed;
uint8_t obs_distance_cm;
uint8_t line_state;
float battery_level;

uint8_t matrix_led_buffer[24];

Ticker ticker;

void set_throw(uint8_t a, uint16_t b, uint16_t c) {
  if (p_odev_throw != NULL) {
    delete p_odev_throw;
  }

  p_odev_throw = new OdevExtendThrow(PB_0, PB_1);
  if (p_odev_throw != NULL) {
    p_odev_throw->set_throw(b, c);
    delete p_odev_throw;
    p_odev_throw = NULL;
  }
  p_odev_throw = new OdevExtendThrow(PA_6, PA_7);
  if (p_odev_throw != NULL) {
    p_odev_throw->set_throw(b, c);
    delete p_odev_throw;
    p_odev_throw = NULL;
  }
}

void get_temperature(uint8_t port, int16_t* temp, int16_t* humi) {
  if (p_temperature != NULL) {
    delete p_temperature;
    p_temperature = NULL;
  }

  if (port == 1) {
    p_temperature = new OdevExtendTemp(PC_5);
  } else if (port == 2) {
    p_temperature = new OdevExtendTemp(PA_3);
  }

  if (p_temperature != NULL) {
    p_temperature->update();
    *temp = p_temperature->get_temperature();
    *humi = p_temperature->get_humidity();
  } else {
    *temp = 0;
    *humi = 0;
  }
}

uint16_t get_sound_sensor_state(uint8_t port) {
  if (p_sound_sensor != NULL) {
    delete p_sound_sensor;
    p_sound_sensor = NULL;
  }

  if (port == 1) {
    p_sound_sensor = new OdevExtendSound(PC_5);
  } else if (port == 2) {
    p_sound_sensor = new OdevExtendSound(PA_3);
  }

  if (p_light_sensor != NULL) {
    return p_sound_sensor->get_sound_state();
  } else {
    return 0;
  }
}

uint16_t get_light_sensor_state(uint8_t port) {
  if (p_light_sensor != NULL) {
    delete p_light_sensor;
    p_light_sensor = NULL;
  }

  if (port == 1) {
    p_light_sensor = new OdevExtendLight(PC_5);
  } else if (port == 2) {
    p_light_sensor = new OdevExtendLight(PA_3);
  }

  if (p_light_sensor != NULL) {
    return p_light_sensor->get_light_state();
  } else {
    return 0;
  }
}

uint16_t get_motion_sensor_state(uint8_t port) {
  if (p_motion_sensor != NULL) {
    delete p_motion_sensor;
    p_motion_sensor = NULL;
  }

  if (port == 1) {
    p_motion_sensor = new OdevExtendMotion(PC_5, PC_4);
  } else if (port == 2) {
    p_motion_sensor = new OdevExtendMotion(PA_3, PA_2);
  }

  if (p_motion_sensor != NULL) {
    return p_motion_sensor->get_motion_state();
  } else {
    return 0;
  }
}

uint16_t get_gas_sensor_state(uint8_t port) {
  if (p_gas_sensor != NULL) {
    delete p_gas_sensor;
    p_gas_sensor = NULL;
  }

  if (port == 1) {
    p_gas_sensor = new OdevExtendGas(PC_5, PC_4);
  } else if (port == 2) {
    p_gas_sensor = new OdevExtendGas(PA_3, PA_2);
  }

  if (p_gas_sensor != NULL) {
    return p_gas_sensor->get_gas_state();
  } else {
    return 0;
  }
}

uint16_t get_flame_sensor_state(uint8_t port) {
  if (p_flame_sensor != NULL) {
    delete p_flame_sensor;
    p_flame_sensor = NULL;
  }

  if (port == 1) {
    p_flame_sensor = new OdevExtendFlame(PC_5, PC_4);
  } else if (port == 2) {
    p_flame_sensor = new OdevExtendFlame(PA_3, PA_2);
  }

  if (p_flame_sensor != NULL) {
    return p_flame_sensor->get_flame_state();
  } else {
    return 0;
  }
}

void set_seg_display(uint8_t port, int16_t dat, uint8_t point) {
  double display_value = 0.0;
  if (p_seg_display != NULL) {
    delete p_seg_display;
    p_seg_display = NULL;
  }

  if (port == 1) {
    p_seg_display = new Odev7SegDisplay(PC_5, PC_4);
  } else if (port == 2) {
    p_seg_display = new Odev7SegDisplay(PA_3, PA_2);
  }

  if (p_seg_display != NULL) {
    display_value = dat;
    display_value = display_value / pow(10.0, point);
    p_seg_display->display(display_value, point);
  }
}

/**
 * 设置蜂鸣器
 */
void set_buzzer_beep(uint16_t freq, uint16_t dura) {
  buzzer.beep(freq, dura * 1000);
}

/**
 * 得到巡线传感器的值
 */
uint16_t get_line_sensor_value(uint8_t num){
	if(num<4)
		return line.getLineSensorValue(num);
	else
		return 0;
}

/**
 * 得到巡线传感器设定的阈值
 */
uint16_t get_line_sensor_level(uint8_t num){
	if(num<4)
		return line.getLineSensorLevel(num);
	else
		return 0;
}

/**
 * 设置巡线传感器的阈值
 */
void set_line_sensor_value(uint8_t num,uint16_t value){
	line.setLineSensorValue(num,value);
}

/**
 * 设置点阵显示第一部分，串口通讯时调用
 */
void set_matrix_led_left(uint8_t* data, uint8_t len) {
  memcpy(matrix_led_buffer, data, 12);
  led.setBuffer(matrix_led_buffer);
  action.set_manual_face(matrix_led_buffer);
}

/**
 * 设置点阵显示第二部分，串口通讯时调用
 */
void set_matrix_led_right(uint8_t* data, uint8_t len) {
  memcpy(matrix_led_buffer + 12, data, 12);
  led.setBuffer(matrix_led_buffer);
  action.set_manual_face(matrix_led_buffer);
}

/**
 * 设置速度值，串口通讯以及自定义表情时调用
 * @param a 左轮速度
 * @param b 右轮速度
 */
void set_motor_speed(int8_t a, int8_t b) {
  float speed_left = a;
  float speed_right = b;
	
  //app陀螺仪计算的速度超过100，这里保险起见加限制
  if(a>100)
	  a=100;
  if(b>100)
	  b=100;
  
  if(a<-100)
	  a=-100;
  if(b<-100)
	  b=-100;  
  
  speed_left = a;
  speed_right = b;

  motion.active();

  motor_speed[0] = speed_left / 100.0;
  motor_speed[1] = speed_right / 100.0;

  motion.set_wheel_speed(motor_speed[0], motor_speed[1]);
}

/**
 * 设置速度值，串口通讯时调用
 * @param a 速度
 */
void set_rocket_speed(uint8_t a) {
  float rocket = a;
  rocket_speed = rocket / 100.0;
  motion.set_rocket_speed(rocket_speed);
}

/**
 * 设置测试模式下，运动模式
 * @param a 速度
 */
void set_test_speed(uint8_t a) {
  float rocket = a;
  rocket_speed = rocket / 100.0;
  motion.set_test_mode_speed(rocket_speed);
}

/**
 * 语音播放回调函数，串口通讯时调用
 * @param a 语音大类文件夹名称
 * @param b 语音文件名称
 */
void set_sound_play(uint8_t a, uint8_t b) {
  sound.play(b, a);
}

/**
 * 设置静默模式
 * @param a 是否开启静默模式
 */
void set_silent_mode(uint8_t a) {
  action.set_silent_mode(a);
}

/**
 * 设置模式
 * @param car_type 车的类型（未使用）
 * @param mode 控制模式
 */
void set_device_mode(int8_t car_type, int8_t mode) {
  // control_mode = mode;
  motion.set_mode(mode);
}

/**
 * 得到传感器存储的值
 */
void get_save_line_sensor_data(uint16_t* data_a, uint16_t* data_b,
						uint16_t* data_c, uint16_t* data_d) {
	
	*data_a = readEEPROMWord(4);
	*data_b = readEEPROMWord(6);
	*data_c = readEEPROMWord(8);
	*data_d = readEEPROMWord(10);;
}
/**
* 设置flash中存储语音的类型,在flash中的0xaa5500xx,其中aa55位标记位，
			AUD_CN_MODE		0x00
			AUD_EN_MODE     0x01
* @return	aud_type;
 */
void set_audio_type(uint8_t aud_type) {
	uint32_t val = 0;
	
	uint16_t get_trig_a = 0x0000;
	uint16_t get_trig_b = 0x0000;
	uint16_t get_trig_c = 0x0000;
	uint16_t get_trig_d = 0x0000;	
				
	if(aud_type!=readEEPROMHalfWord(0))
	{
		get_save_line_sensor_data(&get_trig_a,&get_trig_b,
				&get_trig_c,&get_trig_d);
		
		enableEEPROMWriting();
		wait(1);
		val = 0xaa550000|aud_type;		
		writeEEPROMWord(0, val);
		
		//将设置的阈值存储在flash中
		val = (get_trig_b<<16)|get_trig_a;
		writeEEPROMWord(4, val);
		
		val = (get_trig_d<<16)|get_trig_c;
		writeEEPROMWord(8, val);
		//
		wait(1);
		disableEEPROMWriting();
	}
}

void save_line_sensor_data(uint16_t sv_trig_a,uint16_t sv_trig_b ,
	uint16_t sv_trig_c,uint16_t sv_trig_d) {
		
	uint8_t sv_aud_type	;
	uint32_t val = 0;
	
	uint16_t get_trig_a = 0x0000;
	uint16_t get_trig_b = 0x0000;
	uint16_t get_trig_c = 0x0000;
	uint16_t get_trig_d = 0x0000;	
		
	get_save_line_sensor_data(&get_trig_a,&get_trig_b,
				&get_trig_c,&get_trig_d);
			
	if((get_trig_a!=sv_trig_a)||(get_trig_b!=sv_trig_b)
		||(get_trig_c!=sv_trig_c)||(get_trig_d!=sv_trig_d))
	{
		sv_aud_type=readEEPROMHalfWord(0);
			
		enableEEPROMWriting();
		wait(1);
		val = 0xaa550000|sv_aud_type;		
		writeEEPROMWord(0, val);
		
		//将设置的阈值存储在flash中
				//将设置的阈值存储在flash中
		val = (sv_trig_b<<16)|sv_trig_a;
		writeEEPROMWord(4, val);
		
		val = (sv_trig_d<<16)|sv_trig_c;
		writeEEPROMWord(8, val);
		//
		wait(1);
		disableEEPROMWriting();
	}
}

void Init_all_data(void)
{
	uint32_t val = 0;
	uint16_t def_trig_a = 0x8000;
	uint16_t def_trig_b = 0x8000;
	uint16_t def_trig_c = 0x8000;
	uint16_t def_trig_d = 0x8000;
	
	uint16_t sv_sen_a, sv_sen_b, sv_sen_c, sv_sen_d;
	
	val  =readEEPROMWord(2);
	
	if((val&0xffff)!=0xaa55)
	{
		//注意存储方式为大小端模式
		val = 0xaa550001;
		enableEEPROMWriting();
		wait(1);
		writeEEPROMWord(0, val);
		
		//将设置的阈值存储在flash中
		val = (def_trig_b<<16)|def_trig_a;
		writeEEPROMWord(4, val);
		
		val = (def_trig_d<<16)|def_trig_c;
		writeEEPROMWord(8, val);
		//
		wait(1);
		disableEEPROMWriting();		
	}	
	get_save_line_sensor_data(&sv_sen_a,&sv_sen_b,&sv_sen_c,&sv_sen_d);
	if((sv_sen_a==0)||(sv_sen_b==0)||(sv_sen_c==0)||(sv_sen_d==0))
	{
		//注意存储方式为大小端模式
		val = 0xaa550001;
		enableEEPROMWriting();
		wait(1);
		writeEEPROMWord(0, val);
		
		//将设置的阈值存储在flash中
		val = (def_trig_b<<16)|def_trig_a;
		writeEEPROMWord(4, val);
		
		val = (def_trig_d<<16)|def_trig_c;
		writeEEPROMWord(8, val);
		//
		wait(1);
		disableEEPROMWriting();
	}
	if((sv_sen_a==0xffff)||(sv_sen_b==0xffff)
		||(sv_sen_c==0xffff)||(sv_sen_d==0xffff))
	{
		//注意存储方式为大小端模式
		val = 0xaa550001;
		enableEEPROMWriting();
		wait(1);
		writeEEPROMWord(0, val);
		
		//将设置的阈值存储在flash中
		val = (def_trig_b<<16)|def_trig_a;
		writeEEPROMWord(4, val);
		
		val = (def_trig_d<<16)|def_trig_c;
		writeEEPROMWord(8, val);
		//
		wait(1);
		disableEEPROMWriting();
	}
	
	set_line_sensor_value(0,sv_sen_a);
	set_line_sensor_value(1,sv_sen_b);
	set_line_sensor_value(2,sv_sen_c);
	set_line_sensor_value(3,sv_sen_d);	
	return;
}
/**
 * 查询超声波距离
 * @return 超声波距离
 */
uint8_t get_ultrasonic_distanse() {
  return sonic.get_distance();
}

/**
 * 查询按键的状态
 * @return 按键状态
 */
uint8_t get_key_state() {
  return power_manager.getKeyState();
}

/**
 * 查询巡线传感器的状态
 * @return 巡线传感器状态值
 */
uint8_t get_line_state(void) {
  return line_state;
}

/**
 * 查询flash中存储语音的状态
 * @return  AUD_CN_MODE		0x00
			AUD_EN_MODE     0x01
uint8_t	aud_mode;
 */
uint16_t get_audio_type(void) {
	
	uint16_t get_aud_mode = 0;
	get_aud_mode = readEEPROMHalfWord(2);
	return get_aud_mode;
}

/**
 * 查询是否在充电状态
 * @return  No_Charge_MODE		0x00
			On_Charge_MODE     0x01
 */
uint16_t get_charge_mode(void) {
	
	if(power_manager.getCurrentPowerState()==POWER_CHARGE) 
		return 1;
	return 0;
}

/// 复位进bootloader
void reset_and_goto_bootloader(void) {
  uint8_t* power_state = (uint8_t*)POWER_STATE;
  led.clear();

  power_state[0] = 0x96;
  power_state[1] = 1;
  power_state[2] = 0x01 ^ 0xFF;
  power_state[3] = 0x69;

  NVIC_SystemReset();
}

/**
 * 轮子转速回调函数
 * @param a 左轮转速
 * @param b 右轮转速
 */
void motion_set_motors_pwm(float a, float b) {
  motor_left.setPwm(a);
  motor_right.setPwm(b);
}

void action_set_matrix(const uint8_t* d) {
  memcpy(matrix_led_buffer, d, 24);
  led.setBuffer(matrix_led_buffer);
}

/**
 * 语音播放回调函数，自定义表情动作时调用
 * @param a 语音大类文件夹名称
 * @param b 语音文件名称
 */
void action_set_sound(uint8_t a, uint8_t b) {
  sound.play(a, b);
}


///检查是否在空闲界面，并执行相应的表情动作
void check_normal_no_active()
{
	if(!command.getAppActiveState())
	{
		//app 普通摇杆模式空闲或选择页面空闲或植物
	  if((motion.get_mode() & 0x7f) == 0){
		  if((command.getAppTaskMode()<=1))
			action.idle_action();
		}
	}
}

/// 通讯正常的回调函数，接受到一个正确的数据包时调用
void communication_active() {
  motion.active();
  action.active();  
}

/// 电源开机的回调函数
void power_on() {
  action.power_on_action();
}

/// 电源关机的回调函数
void power_off() {
  motion.set_mode(0);
  motion.set_wheel_speed(0, 0);
  action.power_off_action();
}

void set_charge_state(){
  motion.set_mode(0);
  motion.set_wheel_speed(0, 0);
}
/// 判断电池电压，并执行相应的表情动作
uint8_t check_battery_level() {
  battery_level = power_manager.get_bat_level();
  //约3.3v
  if (battery_level < 0.507) {
    action.power_low_action();
	
	  //约3.0v自动关机
	if(battery_level < 0.501){
		power_manager.set_power_state(POWER_OFF);
	}
	return 1;  	  
  }
  return 0;
}

/// 短按按钮的回调函数，在非编程界面都应该执行
void key_press() {
  if(command.getAppConnectState()==0)
  {
	   //app 普通摇杆模式空闲或选择页面空闲或植物
	  if(command.getAppTaskMode()<=1)
	  {
		if(((motion.get_mode() & 0x7f)==0)&&(!command.getAppActiveState()))
		{	  
			action.set_idle_speed(50);
			action.key_action();
		}
		else
		{
			action.set_idle_speed(0);
			action.key_action();
		}
	  }
  }
  else
  {
	  if((motion.get_mode() & 0x7f) == 0)//
		action.set_idle_speed(50);
	  else
		action.set_idle_speed(0);  
	  action.key_action();
  }
}

/**
 * 串口查询电池电压的回调函数
 * @return 电池电压0~100
 */
uint8_t get_battery_level() {
  return power_manager.get_bat_level() * 100.0;
}

uint8_t motion_get_test_mode(){
	return command.get_test_mode();
}

/// 添加串口通信回调函数
inline void add_serial_callbacks(void) {
  command.add_motor_speed_callback(&set_motor_speed);
  command.add_rocket_speed_callback(&set_rocket_speed);

  command.add_sound_callback(&set_sound_play);
  command.add_sound_off_callback(&set_silent_mode);

  command.add_matrix_left_callback(&set_matrix_led_left);
  command.add_matrix_right_callback(&set_matrix_led_right);

  command.add_throw_callback(&set_throw);
  command.add_get_line_callback(&get_line_state);
  command.add_set_mode_callback(&set_device_mode);

  command.add_get_ultrasonic_callback(&get_ultrasonic_distanse);
  command.add_get_key_state_callback(&get_key_state);
  command.add_get_battery_callback(&get_battery_level);

  command.add_set_program_mode_callback(&reset_and_goto_bootloader);
  command.add_set_active_callback(&communication_active);

  command.add_get_temperature_callback(get_temperature);
  command.add_get_light_sensor_state_callback(get_light_sensor_state);
  command.add_get_sound_sensor_state_callback(get_sound_sensor_state);
  command.add_get_motion_sensor_state_callback(get_motion_sensor_state);
  command.add_get_gas_sensor_state_callback(get_gas_sensor_state);
  command.add_get_flame_sensor_state_callback(get_flame_sensor_state);

  command.add_set_beep_freq_callback(set_buzzer_beep);
  command.add_set_seg_display_callback(set_seg_display);
  
  command.add_set_aud_type_callback(&set_audio_type);
  command.add_get_aud_type_callback(&get_audio_type);
  
  command.add_get_charge_mode_callback(&get_charge_mode);
  
  command.add_set_test_mode_speed_callback(&set_test_speed);
  
  command.add_get_line_sensor_callback(&get_line_sensor_value);
  command.add_get_line_sensor_level_callback(&get_line_sensor_level);
  
  command.add_set_line_sensor_callback(&set_line_sensor_value);
  
  command.add_sv_line_sensor_data_callback(&save_line_sensor_data);
  
}

/**
 * 临时测试
 * @return 
 */
void Test(void)
{	
}
//end test

int main() {
  uint32_t delay_50ms, delay_1s,delay_battery_30s;
  uint32_t delay_app_no_active_30s;
  uint32_t delay_charge_1s;
	
  __enable_irq();
  uint8_t* power_stat_from_bootloader = (uint8_t*)POWER_STATE;

  Init_all_data();
  
  add_serial_callbacks();

  power_manager.add_power_on_callback(&power_on);
  power_manager.add_power_off_callback(&power_off);
  power_manager.add_key_press_callback(&key_press);
  power_manager.add_power_charge_callback(&set_charge_state);

  motion.add_set_pwn_callback(&motion_set_motors_pwm);
  motion.add_get_test_mode_callback(&motion_get_test_mode);

  action.add_set_led_matrix_callback(&action_set_matrix);
  action.add_set_motor_speed_callback(&set_motor_speed);
  action.add_set_sound_callback(&action_set_sound);

  /*判断是否是由Bootloader更新完程序后跳转过来的，如果是则直接将电源打开*/
  if (power_stat_from_bootloader[0] == 0x69 && power_stat_from_bootloader[1] == 0x02 && power_stat_from_bootloader[2] == (0x02 ^ 0xFF) && power_stat_from_bootloader[3] == 0x96) {
    power_manager.set_power_on();
  }

  wait(1);

  led.init();
  sound.init();

  rocket_speed = 1.0;

  // motion.set_mode(2);

  wait(0.2);
  
  delay_50ms = us_ticker_read();
  ticker.attach(&my_ticker_count, 0.01); //call it every 0.01ms
  delay_1s = my_ticker_read();
  delay_battery_30s = my_ticker_read();
  delay_app_no_active_30s = my_ticker_read();
  delay_charge_1s = my_ticker_read();
  
  while (true) {
    power_manager.loop();
    led.display();
	  		
    action.update(obs_distance_cm, motion.get_mode());

    motor_left.update();
    motor_right.update();

    buzzer.update();
		
	if(power_manager.getCurrentPowerState()==POWER_CHARGE)			
	{
 		command.update(); 
		motion.update(line_state, obs_distance_cm);		

		if ((my_ticker_read() - delay_charge_1s) > 100) //1s
		{
			action.charge_mode_action();
			delay_charge_1s = my_ticker_read();
		}
		continue;		
	}
	  			  
    if (power_manager.getPowerState()) {
      if(power_manager.getCurrentPowerState()==POWER_WAIT_OFF) 		
		command.update(); 
      sonic.update();
      line.update();
      motion.update(line_state, obs_distance_cm);
	  		  
      if ((us_ticker_read() - delay_50ms) > 5000){
        line_state = line.getState();
        obs_distance_cm = sonic.get_distance();
        delay_50ms = us_ticker_read();
      }
	
      if ((my_ticker_read() - delay_1s) > 100) {
        if ((motion.get_mode() & 0x7f) != 0) {
          action.active();
        }
        //check_battery_level();
        delay_1s = my_ticker_read();
      }
	  
	  if ((my_ticker_read() - delay_battery_30s) > 3000) {
        if(check_battery_level())
		  delay_app_no_active_30s = my_ticker_read()-1500;
        delay_battery_30s = my_ticker_read();
      }	

	  if ((my_ticker_read() - delay_app_no_active_30s) > 3000) {		  
		 check_normal_no_active();	
		 delay_app_no_active_30s = my_ticker_read();	
		 delay_battery_30s = my_ticker_read();
	  }	  
    }
  }
}
