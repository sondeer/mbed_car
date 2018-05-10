/**
 * @file odev_command.cpp
 * @author gongpan
 * @version 1.1
 * @date 2017-06-22
 * @brief 蓝牙串口命令解析
 */

#include "odev_command.h"

#define PACKET_HEADER 0xEA
#define GET_PACKET_LENGTH(a) ((a)&0x1f)
#define GET_PACKET_CMD(a) (uint8_t)(((a) >> 5) & 0x07)
#define GEN_PACKET_CMDLENGTH(a, b) (uint8_t)(((a) << 5) | (b))

#define CMD_ACK 0
#define CMD_GOTO_APP 1
#define CMD_SET_ADDR 2
#define CMD_SET_DATA 3
#define CMD_GET_VER 4
#define CMD_CHECK_AND_PRO 5
#define CMD_SET_FIRMWARE_INF 6
#define CMD_EXTEND 7

#define CMD_EXT_DATA_NOACK 0
#define CMD_EXT_DATA_HASACK 1
#define CMD_EXT_LEAK_PACKET 2

#define CMD_EXT_SET 3
#define CMD_EXT_GET 4
#define CMD_EXT_ROCKER 5

#define CMD_EXT_TS_MODE 15

#define SET_MOTOR_SPEED 1
#define SET_MOTOR_PWM 2
#define SET_LED_RGB 3
#define SET_LED_PATTERN 4
#define SET_BUZZER_PWM 5
#define SET_ROCKER_SPEED 6
#define SET_DEVICE_MODE 7
#define SET_AUDIO_INDEX 8
#define SET_LED_PATTERN2 9
#define SET_THROWER_STATE 10
#define SET_AUDIO_OFF 11
#define SET_SEG_DISPLAY 12
#define SET_BEEP_FREQ 13
#define SET_AUD_TYPE	14
#define SET_APP_TASK_MODE	15
#define SET_LINE_SEN_LEVEL   16

#define SET_PROGRAM_MODE 0x80

#define GET_MOTOR_SPEED 1
#define GET_MOTOR_PWM 2
#define GET_LED_RGB 3
#define GET_LED_TUAN 4
#define GET_BUZZER_PWM 5
#define GET_ROCKER_SPEED 6
#define GET_DEVICE_MODE 7

#define GET_ULTRASONIC_DIS 0x10
#define GET_LINE_PATROL 0x11
#define GET_GYRO 0x12
#define GET_BATTERY_STATE 0x13
#define GET_KEY_STATE 0x14
#define GET_THROW_STATE 0x15

#define GET_TEMPERATURE 0x20
#define GET_LIGHT_SENSOR 0x21
#define GET_SOUND_SENSOR 0x22
#define GET_MOTION_SENSOR 0x23
#define GET_GAS_SENSOR 0x24
#define GET_FLAME_SENSOR 0x25
#define GET_AUD_TYPE	0x26
#define GET_APP_TASK_MODE	0x27
#define GET_LINE_SEN_VALUE  0x28
#define GET_LINE_SEN_LEVEL  0x29

#define GET_ALL 0xFF

#define COMM_WAIT_SOF 0x00
#define COMM_RECV_INX 0x01
#define COMM_RECV_LEN 0x02
#define COMM_RECV_DATA 0x03
#define COMM_PARSE_DATA 0x04
#define COMM_GET_CHECKSUM 0x05

namespace Odev {

OdevCommand::OdevCommand(PinName tx, PinName rx) : pc(tx, rx, 115200) {
  pc.attach(callback(this, &OdevCommand::rx_irq), RawSerial::RxIrq);
  //prev_update_tick = us_ticker_read();
	watchdog_update_tick = us_ticker_read();
    state = COMM_WAIT_SOF;
	pre_app_active_tick = us_ticker_read();
	pre_app_connect_tick = us_ticker_read();
	app_task_mode = 0;
	on_test_mode = 0;
}

void OdevCommand::rx_irq() {
  rx_buffer.push(pc.getc());
}

void OdevCommand::update() {
  uint8_t temp;
  uint8_t checksum = 0;
  uint8_t tx_buffer[16];
  static uint8_t serialCountId  = 0;
	
  while (rx_buffer.pop(temp) && state != COMM_PARSE_DATA) {
    switch (state) {
      case COMM_WAIT_SOF:
        if (temp == PACKET_HEADER) {
          state = COMM_RECV_INX;
        }
        rx_frame.header = temp;
        break;

      case COMM_RECV_INX:
        state = COMM_RECV_LEN;
        rx_frame.serialNum = temp;
        break;

      case COMM_RECV_LEN:
        state = COMM_RECV_DATA;
        rx_frame.cmdAndLength = temp;
        data_left = 0x1F & temp;
        if (data_left > 17) {
          state = COMM_WAIT_SOF;
        } else if(data_left == 0){
					state = COMM_GET_CHECKSUM;
				}
        break;
      case COMM_RECV_DATA:
        rx_frame.data[(rx_frame.cmdAndLength & 0x1f) - data_left] = temp;
        data_left--;

        if (data_left == 0) {
          state = COMM_GET_CHECKSUM;
        }
        break;

      case COMM_GET_CHECKSUM:
        checksum = getCheckSum(&(rx_frame), (rx_frame.cmdAndLength & 0x1f) + 3);
        if (checksum == temp) {
          state = COMM_PARSE_DATA;
        } else {
          state = COMM_WAIT_SOF;
        }

      default:
        break;
    }
		
		watchdog_update_tick = us_ticker_read();
  }

  if (state == COMM_PARSE_DATA) {
    recvPacket();
    state = COMM_WAIT_SOF;

    //last_update_tick = us_ticker_read();

    if (on_active) {
      on_active();
    }
  }
   #if 0
   if (us_ticker_read() - last_command_update_tick > 10000)
   {
     tx_buffer[0] = 0x04;
	 tx_buffer[1] = GET_ALL;
     if (on_get_mode) {
        tx_buffer[2] = on_get_mode();
      } else {
        tx_buffer[2] = 0;
      }

      if (on_get_ultrasonic) {
        tx_buffer[3] = on_get_ultrasonic();
      } else {
        tx_buffer[3] = 0;
      }

      if (on_get_line) {
        tx_buffer[4] = on_get_line();
      } else {
        tx_buffer[4] = 0;
      }

      if (on_get_battery) {
        tx_buffer[5] = on_get_battery();
      } else {
        tx_buffer[5] = 0;
      }

      if (on_get_key_state) {
        tx_buffer[6] = on_get_key_state();
      } else {
        tx_buffer[6] = 0;
      }

      if (on_get_throw_state) {
        tx_buffer[7] = on_get_throw_state();
      } else {
        tx_buffer[7] = 0;
      }
	  /*
	  if (on_get_aud_type) {
			tx_buffer[8] = on_get_aud_type();
      } else {
			tx_buffer[8] = 0;
	  }
	  */
      sendFrame(CMD_EXTEND, serialCountId, tx_buffer, 8);
	  serialCountId++;
      last_command_update_tick = us_ticker_read();
  }
  #endif

}

void OdevCommand::recvPacket() {
	uint8_t tx_buffer[16];
  uint8_t cmd = GET_PACKET_CMD(rx_frame.cmdAndLength);
  switch (cmd) {
    case CMD_ACK:
      break;
    case CMD_GOTO_APP:
      break;
    case CMD_SET_ADDR:
      break;
    case CMD_SET_DATA:
      break;
    case CMD_GET_VER:
			*((uint32_t * )tx_buffer) = *((uint32_t * )0x80040C0);
		  *((uint32_t * )(tx_buffer+4)) = *((uint32_t * )0x80040C4);
		  tx_buffer[8] = 2;
			sendFrame(CMD_GET_VER, rx_frame.serialNum, tx_buffer, 9);
      break;
    case CMD_CHECK_AND_PRO:
      break;
    case CMD_SET_FIRMWARE_INF:
      break;
    case CMD_EXTEND: {
      recvExtPacket();
    } break;
    default:
      break;
  }
}

void OdevCommand::recvExtPacket() {
	//app 连接
  pre_app_connect_tick = us_ticker_read();
  
  if(on_get_charge_mode)
  {
	  if(on_get_charge_mode()&&(rx_frame.data[0]!=CMD_EXT_TS_MODE))//
		  return;
  }
//  if(on_test_mode)
//  {
//	  if(rx_frame.data[0]!=CMD_EXT_TS_MODE)
//		  return;
//  }
	  
  switch (rx_frame.data[0]) {
    case CMD_EXT_SET:
      setState();
      break;
    case CMD_EXT_GET:
      getState();
      break;
	case CMD_EXT_TS_MODE:
	  entry_test_mode();	
	  break;
    default:
      break;
  }
}

void OdevCommand::set_test_mode(uint8_t mode)
{
	on_test_mode = mode;
}

uint8_t OdevCommand::get_test_mode(void)
{
	return on_test_mode;
}

void OdevCommand::entry_test_mode(void)
{
	
	if(rx_frame.data[1]==1)
	{
		set_test_mode(1);	
    }else{
		set_test_mode(0);
		if (on_set_test_mode_speed) 
			on_set_test_mode_speed(0);
	}	
}

void OdevCommand::setState() {
  uint8_t ack = 0;
  uint8_t extend_port = 0;
  uint16_t extend_freq = 0;
  uint16_t extend_count = 0;
  int16_t seg_data = 0;
  uint8_t seg_point = 0;
  uint16_t beep_freq = 0;
  uint16_t beep_duration = 0;
  uint8_t aud_type_data = 0;
  uint8_t app_task_data = 0;
  uint16_t line_sen_a=0x8000;
  uint16_t line_sen_b=0x8000;
  uint16_t line_sen_c=0x8000;
  uint16_t line_sen_d=0x8000;
	
  static uint8_t lastserialNum = 0;
  static uint8_t lastcommand = 0;

  //检查是否有app连接但是不操作状态,不考虑设置模式命令
  if(rx_frame.data[1]!=SET_DEVICE_MODE)
	pre_app_active_tick = us_ticker_read();

  if((lastserialNum!=rx_frame.serialNum)||(lastcommand!=rx_frame.data[1]))
  {
	  switch (rx_frame.data[1]) {
		case SET_MOTOR_SPEED:
		  if (on_set_motor_speed) {
			on_set_motor_speed(rx_frame.data[2], rx_frame.data[3]);
		  }
		  break;
		case SET_DEVICE_MODE:
		  if (on_set_mode) {
			on_set_mode(rx_frame.data[2], rx_frame.data[3]);
		  }
		  break;
		case SET_ROCKER_SPEED:
		  if (on_set_rocket_speed) {
			on_set_rocket_speed(rx_frame.data[2]);
		  }
		  break;
		case SET_LED_PATTERN:
		  if (on_set_matrix_left) {
			on_set_matrix_left(rx_frame.data + 2, 12);
		  }
		  break;
		case SET_LED_PATTERN2:
		  if (on_set_matrix_right) {
			on_set_matrix_right(rx_frame.data + 2, 12);
		  }
		  break;
		case SET_AUDIO_INDEX:
		  if (on_set_sound) {
			on_set_sound(rx_frame.data[2], rx_frame.data[3]);
		  }
		  break;

		case SET_AUDIO_OFF:
		  if (on_set_silent_mode) {
			on_set_silent_mode(rx_frame.data[2]);
		  }
		  break;
		case SET_THROWER_STATE:
		  if (on_set_throw) {
			if ((rx_frame.data[2] == 1) || (rx_frame.data[2] == 2)) {
			  extend_port = rx_frame.data[2];
			  extend_freq = rx_frame.data[4];
			  extend_freq = extend_freq * 256 + rx_frame.data[3];
			  extend_count = rx_frame.data[6];
			  extend_count = extend_count * 256 + rx_frame.data[5];
			  on_set_throw(extend_port, extend_freq, extend_count);
			}
		  }
		  break;
		case SET_SEG_DISPLAY:
		  if (on_set_seg_display) {
			extend_port = rx_frame.data[2];
			seg_data = rx_frame.data[4];
			seg_data = seg_data * 256 + rx_frame.data[3];
			seg_point = rx_frame.data[5];
			on_set_seg_display(extend_port, seg_data, seg_point);
		  }
		  break;
		case SET_BEEP_FREQ:
		  if (on_set_beep_freq) {
			beep_freq = rx_frame.data[3];
			beep_freq = beep_freq * 256 + rx_frame.data[2];
			beep_duration = rx_frame.data[5];
			beep_duration = beep_duration * 256 + rx_frame.data[4];
			on_set_beep_freq(beep_freq, beep_duration);
		  }
		  break;
		case SET_PROGRAM_MODE:
		  if (on_set_program_mode) {
			on_set_program_mode();
		  }
		  break;
		case SET_AUD_TYPE:
		  if (on_set_aud_type) {
			aud_type_data = rx_frame.data[2];
			on_set_aud_type(aud_type_data);
		  }
		case SET_APP_TASK_MODE:		  
		  app_task_data = rx_frame.data[2];
		  setAppTaskMode(app_task_data);
		  break;
		case SET_LINE_SEN_LEVEL:
		  if (on_get_line_sensor_value) {
			line_sen_a = rx_frame.data[3];
			line_sen_a = line_sen_a * 256 + rx_frame.data[2];
			line_sen_b = rx_frame.data[5];
			line_sen_b = line_sen_b * 256 + rx_frame.data[4];
			line_sen_c = rx_frame.data[7];
			line_sen_c = line_sen_c * 256 + rx_frame.data[6];
			line_sen_d = rx_frame.data[9];
			line_sen_d = line_sen_d * 256 + rx_frame.data[8];
			on_set_line_sensor_value(0,line_sen_a);
			on_set_line_sensor_value(1,line_sen_b);
			on_set_line_sensor_value(2,line_sen_c);
			on_set_line_sensor_value(3,line_sen_d);
			on_save_line_sensor_data(line_sen_a,line_sen_b,line_sen_c,line_sen_d);
		  }
		  break;
		default:
		  break;
	  }
  }
  lastserialNum = rx_frame.serialNum;
  lastcommand = rx_frame.data[1];
  
  sendFrame(CMD_ACK, rx_frame.serialNum, &ack, 1);
}

void OdevCommand::getState() {
  uint8_t tx_buffer[16];

  tx_buffer[0] = rx_frame.data[0];
  tx_buffer[1] = rx_frame.data[1];

  uint8_t extend_port = 0;

  switch (rx_frame.data[1]) {
    case GET_DEVICE_MODE:
      if (on_get_mode) {
        tx_buffer[2] = on_get_mode();
      } else {
        tx_buffer[2] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 3);
      break;

    case GET_ULTRASONIC_DIS:
      if (on_get_ultrasonic) {
        tx_buffer[2] = on_get_ultrasonic();
      } else {
        tx_buffer[2] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 3);
      break;

    case GET_LINE_PATROL:
      if (on_get_line) {
        tx_buffer[2] = on_get_line();
      } else {
        tx_buffer[2] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 3);
      break;

    case GET_BATTERY_STATE:
      if (on_get_battery) {
        tx_buffer[2] = on_get_battery();
      } else {
        tx_buffer[2] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 3);
      break;

    case GET_KEY_STATE:
      if (on_get_key_state) {
        tx_buffer[2] = on_get_key_state();
      } else {
        tx_buffer[2] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 3);
      break;

    case GET_THROW_STATE:
      if (on_get_throw_state) {
        tx_buffer[2] = on_get_throw_state();
      } else {
        tx_buffer[2] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 3);
      break;

    case GET_TEMPERATURE:
      if (on_get_temperature) {
        int16_t temperature, humidity;
        extend_port = rx_frame.data[2];
        on_get_temperature(extend_port, &temperature, &humidity);
        tx_buffer[2] = temperature & 0xFF;
        tx_buffer[3] = temperature >> 8;

        tx_buffer[4] = humidity & 0xFF;
        tx_buffer[5] = humidity >> 8;

      } else {
        tx_buffer[2] = 0;
        tx_buffer[3] = 0;
        tx_buffer[4] = 0;
        tx_buffer[5] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 6);

      break;
    case GET_LIGHT_SENSOR:
      if (on_get_light_sensor_state) {
        extend_port = rx_frame.data[2];

        uint16_t temp = on_get_light_sensor_state(extend_port);
        tx_buffer[2] = temp & 0xFF;
        tx_buffer[3] = temp >> 8;
      } else {
        tx_buffer[2] = 0;
        tx_buffer[3] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 4);

      break;
    case GET_SOUND_SENSOR:
      if (on_get_sound_sensor_state) {
        extend_port = rx_frame.data[2];
        uint16_t temp = on_get_sound_sensor_state(extend_port);
        tx_buffer[2] = temp & 0xFF;
        tx_buffer[3] = temp >> 8;
      } else {
        tx_buffer[2] = 0;
        tx_buffer[3] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 4);

      break;
    case GET_MOTION_SENSOR:
      if (on_get_motion_sensor_state) {
        extend_port = rx_frame.data[2];
        uint16_t temp = on_get_motion_sensor_state(extend_port);
        tx_buffer[2] = temp & 0xFF;
        tx_buffer[3] = temp >> 8;
      } else {
        tx_buffer[2] = 0;
        tx_buffer[3] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 4);

      break;
    case GET_FLAME_SENSOR:
      if (on_get_flame_sensor_state) {
        extend_port = rx_frame.data[2];
        uint16_t temp = on_get_flame_sensor_state(extend_port);
        tx_buffer[2] = temp & 0xFF;
        tx_buffer[3] = temp >> 8;
      } else {
        tx_buffer[2] = 0;
        tx_buffer[3] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 4);

      break;
    case GET_GAS_SENSOR:
      if (on_get_gas_sensor_state) {
        extend_port = rx_frame.data[2];
        uint16_t temp = on_get_gas_sensor_state(extend_port);
        tx_buffer[2] = temp & 0xFF;
        tx_buffer[3] = temp >> 8;
      } else {
        tx_buffer[2] = 0;
        tx_buffer[3] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 4);

      break;
	case GET_AUD_TYPE:
	  if (on_get_aud_type) {
			tx_buffer[2] = on_get_aud_type();
      } else {
			tx_buffer[2] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 3);
	  break;
	case GET_APP_TASK_MODE:	 
	  tx_buffer[2] = getAppTaskMode();      
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 3);
	  break;
	case GET_LINE_SEN_VALUE:
	  if (on_get_line_sensor_value)
	  {
		  uint16_t temp = on_get_line_sensor_value(0);
		  tx_buffer[2] = temp & 0xFF;
		  tx_buffer[3] = temp >> 8;
		
		  temp = on_get_line_sensor_value(1);
		  tx_buffer[4] = temp & 0xFF;
		  tx_buffer[5] = temp >> 8;
		  
		  temp = on_get_line_sensor_value(2);
		  tx_buffer[6] = temp & 0xFF;
		  tx_buffer[7] = temp >> 8;
		  
		  temp = on_get_line_sensor_value(3);
		  tx_buffer[8] = temp & 0xFF;
		  tx_buffer[9] = temp >> 8;
	  }
	  sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 11);
	  break;
	case GET_LINE_SEN_LEVEL:
	  if (on_get_line_sensor_level)
	  {
		  uint16_t temp = on_get_line_sensor_level(0);
		  tx_buffer[2] = temp & 0xFF;
		  tx_buffer[3] = temp >> 8;
		
		  temp = on_get_line_sensor_level(1);
		  tx_buffer[4] = temp & 0xFF;
		  tx_buffer[5] = temp >> 8;
		  
		  temp = on_get_line_sensor_level(2);
		  tx_buffer[6] = temp & 0xFF;
		  tx_buffer[7] = temp >> 8;
		  
		  temp = on_get_line_sensor_level(3);
		  tx_buffer[8] = temp & 0xFF;
		  tx_buffer[9] = temp >> 8;
	  }
	  sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 11);
	  break;
    case GET_ALL:
      if (on_get_mode) {
        tx_buffer[2] = on_get_mode();
      } else {
        tx_buffer[2] = 0;
      }

      if (on_get_ultrasonic) {
        tx_buffer[3] = on_get_ultrasonic();
      } else {
        tx_buffer[3] = 0;
      }

      if (on_get_line) {
        tx_buffer[4] = on_get_line();
      } else {
        tx_buffer[4] = 0;
      }

      if (on_get_battery) {
        tx_buffer[5] = on_get_battery();
      } else {
        tx_buffer[5] = 0;
      }

      if (on_get_key_state) {
        tx_buffer[6] = on_get_key_state();
      } else {
        tx_buffer[6] = 0;
      }

      if (on_get_throw_state) {
        tx_buffer[7] = on_get_throw_state();
      } else {
        tx_buffer[7] = 0;
      }
	  if (on_get_aud_type) {
			tx_buffer[8] = on_get_aud_type();
      } else {
			tx_buffer[8] = 0;
      }
      sendFrame(CMD_EXTEND, rx_frame.serialNum, tx_buffer, 9);
      break;

    default:
      break;
  }
}

void OdevCommand::sendFrame(uint8_t cmd, uint8_t sNum, uint8_t* data, int length) {
  union {
    uint8_t byte_data[20];
    struct frame tx_frame;
  } frame_union;

  frame_union.tx_frame.header = PACKET_HEADER;
  frame_union.tx_frame.serialNum = sNum;
  frame_union.tx_frame.cmdAndLength = GEN_PACKET_CMDLENGTH(cmd, length);
  memcpy(frame_union.tx_frame.data, data, length);

  frame_union.tx_frame.data[length] = getCheckSum(&(frame_union.tx_frame), length + 3);

  for (int i = 0; i < length + 4; i++) {
    while (!pc.writeable())
      ;

    pc.putc(frame_union.byte_data[i]);
  }
}

uint8_t OdevCommand::getCheckSum(void* buff, int size) {
  uint8_t sum = 0;
  uint8_t* p = (uint8_t*)buff;
  for (int i = 0; i < size; i++) {
    sum ^= p[i];
  }
  return sum;
}

uint8_t OdevCommand::getAppActiveState() {
	if((us_ticker_read()-pre_app_active_tick>3000000)
		&&(us_ticker_read()-pre_app_connect_tick)<1000000)		
		return 0;
	else
		return 1;
}

uint8_t OdevCommand::getAppConnectState() {
	//串口超过1s没有数据认为没有连接app
	if((us_ticker_read()-pre_app_connect_tick)<1000000)		
		return 0;
	else
		return 1;
}



void OdevCommand::setAppTaskMode(uint8_t mode) {	
	//0--start 1--遥控，2--编程，3--cloud，4--task任务……
	app_task_mode = mode;
}

uint8_t OdevCommand::getAppTaskMode() {
	return app_task_mode;
}
}

//end 
