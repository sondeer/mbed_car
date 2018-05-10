#include "odev_motion.h"

namespace Odev {

OdevMotionControl::OdevMotionControl() {
  motion_mode = 0;
  last_update_tick = us_ticker_read();
  prev_roam_back_tick = us_ticker_read();
  prev_roam_turn_tick = us_ticker_read();
  prev_active_tick = us_ticker_read();
  test_mode_tick = us_ticker_read();
	
  speed_left = 0.0;
  speed_right = 0.0;
  rocket_speed = 0.5;
  roam_mode_random_dir = 1;
  line_mode_last_turn_dir = 1;
  roam_state = ROAM_NORMAL;

  prev_line_sensor_error = 0.0;
  prev_line_angluar_speed = 0.0;
}

void OdevMotionControl::update(uint8_t line, uint8_t sonar) {
  uint8_t mode = motion_mode & 0x7f;
  // data[8] = motion_mode;
  if(get_test_mode()){
	  test_mode_process();			
  }
  else{
     switch (mode)
     {
		/*普通模式*/
		case 0:
		  normal_mode_process(line, sonar);
		  break;

		/*漫游模式*/
		case 1:
		  roam_mode_process(line, sonar);
		  break;

		/*巡线模式*/
		case 2:
		  line_mode_process(line, sonar);
		  break;

		/*轨迹模式*/
		case 3:
		  normal_mode_process(line, sonar);
		  break;

		/*引导模式*/
		case 4:
		  follow_mode_process(line, sonar);
		  break;
		default:
		  break;
	 }
  }
}

void OdevMotionControl::set_velocity(float linear, float angluar) {
  if (set_pwm) {
    set_pwm(angluar + linear, angluar - linear);
  }
}

void OdevMotionControl::set_wheel_speed(float left, float right) {
  speed_left = left;
  speed_right = right;
}

void OdevMotionControl::set_rocket_speed(float speed) {
  rocket_speed = speed;
}

void OdevMotionControl::set_mode(uint8_t mode) {
  motion_mode = mode;
}

uint8_t OdevMotionControl::get_mode() {
  return motion_mode;
}

uint8_t OdevMotionControl::has_teltop_velocity() {
  if (speed_left != 0 || speed_right != 0) {
    if (set_pwm) {
      set_pwm(speed_left, speed_right);
    }
    return 1;
  } else {
    return 0;
  }
}

void OdevMotionControl::normal_mode_process(uint8_t line, uint8_t sonar) {
  uint8_t avoid_abs = motion_mode & 0x80;
  float speed[2];

  speed[0] = speed_left;
  speed[1] = speed_right;

  if (avoid_abs && speed_left > 0 && speed_right < 0) {
    if (sonar < 20) {
      speed[0] = 0;
      speed[1] = 0;
    } else if (sonar < 40) {
      speed[0] *= (((float)sonar - 20.0) / 20.0);
      speed[1] *= (((float)sonar - 20.0) / 20.0);
    }
  }

  if ((us_ticker_read() - prev_active_tick) > 3000000) {
    speed[0] = 0;
    speed[1] = 0;
	  
	speed_left = 0;
	speed_right =0;
  }

  if (set_pwm) {
    set_pwm(speed[0], speed[1]);
  }
}

void OdevMotionControl::roam_mode_process(uint8_t line, uint8_t sonar) {
  float roam_linear_vel = rocket_speed;
  float roam_angluar_vel = rocket_speed * 0.05 + 0.2;

  if (has_teltop_velocity()) {
    return;
  }

  switch (roam_state) {
    case ROAM_NORMAL:
      if (sonar < 20 && ((us_ticker_read() - prev_roam_back_tick) > 1000000)) {
        /*当距离小于20厘米时，进入后退模式*/
        roam_state = ROAM_GO_BACK;
        last_update_tick = us_ticker_read();
        prev_roam_back_tick = us_ticker_read();
        break;
      } else if (sonar < 40) {
        /*当距离小于40厘米时，进入旋转模式*/
        roam_state = ROAM_RANDOM_TURN;
        last_update_tick = us_ticker_read();
        srand(us_ticker_read());
        roam_mode_random_dir = (rand() % 2 > 0) ? 1 : -1;
        break;
      }
      set_velocity(roam_linear_vel, 0);
      break;

    case ROAM_RANDOM_TURN:
      if (sonar > 40 && ((us_ticker_read() - prev_roam_turn_tick) > 1000000)) {
        /*当距离大于40厘米，进入*/
        roam_state = ROAM_NORMAL;
        prev_roam_turn_tick = us_ticker_read();
        break;
      } else if ((us_ticker_read() - prev_roam_turn_tick) > 2000000) {
        roam_mode_random_dir = -roam_mode_random_dir;
        prev_roam_turn_tick = us_ticker_read();
      }
      set_velocity(0, roam_mode_random_dir * roam_angluar_vel);
      break;

    case ROAM_GO_BACK:
      if (sonar > 20 && ((us_ticker_read() - prev_roam_back_tick) > 1000000)) {
        roam_state = ROAM_RANDOM_TURN;
        prev_roam_back_tick = us_ticker_read();
        prev_roam_turn_tick = us_ticker_read();
        break;
      }
      /*
if (sonar > 40) {
roam_state = ROAM_NORMAL;
break;
} */

      set_velocity(-roam_linear_vel, 0);
      break;
  }
}

int8_t OdevMotionControl::getLineSensorError(uint8_t line) {
  switch (line) {
    case 1:
      return 3;
    case 3:
      return 2;
    case 2:
    case 7:
      return 1;

    case 4:
    case 14:
      return -1;
    case 12:
      return -2;
    case 8:
      return -3;

    default:
      return 0;
  }
}

void OdevMotionControl::line_mode_process(uint8_t line, uint8_t sonar) {
  float sensor_error;
  float angular_speed = 0.0;
  float angular_speed_i = 0.0;

  float line_linear_vel = 0.5;//0.4;

  uint8_t avoid_abs = motion_mode & 0x80;

  sensor_error = getLineSensorError(line);
  angular_speed = fabs(sensor_error) / 3.0 * 0.35;

  if (sensor_error != 0) {
    angular_speed_i += 0.0001;
    if (angular_speed_i > 0.1) {
      angular_speed_i = 0.1;
    }
  } else {
    angular_speed_i = 0.0;
  }

  angular_speed += angular_speed_i;

  if (has_teltop_velocity()) {
    return;
  }
  if (avoid_abs && sonar < 30) {
    set_velocity(0.0, 0.0);
    return;
  }
  if (sensor_error < 0) {
    line_mode_last_turn_dir = 1;
    set_velocity(line_linear_vel, angular_speed);
  } else if (sensor_error > 0) {
    line_mode_last_turn_dir = 2;
    set_velocity(line_linear_vel, -angular_speed);
  } else {
    if (line == 0) {
      if (line_mode_last_turn_dir == 1) {
        set_velocity(line_linear_vel, 0.5);//0.4);//
      } else if (line_mode_last_turn_dir == 2) {
        set_velocity(line_linear_vel, -0.5);//-0.4);//
      }
    } else {
      set_velocity(line_linear_vel, 0.0);
    }
  }

  prev_line_sensor_error = sensor_error;
  prev_line_angluar_speed = angular_speed;
}

void OdevMotionControl::follow_mode_process(uint8_t line, uint8_t sonar) {
  /*
  if((sonar <= 10)||(sonar >= 25)) {
    set_velocity(0, 0);
  }else if (sonar > 10 && sonar < 15){
          set_velocity(-rocket_speed, 0);
  } else if (sonar < 25 && sonar > 20) {
          set_velocity(rocket_speed, 0);
  } */

  if (has_teltop_velocity()) {
    return;
  }

  if ((sonar >= 10) && (sonar <= 50)) {
    set_velocity(rocket_speed, 0);
  } else {
    set_velocity(0, 0);
  }
}

void OdevMotionControl::test_mode_process(void) {

  static float speed_test_value= 100;

  if ((us_ticker_read() - test_mode_tick) > 30000000) {	  
	speed_test_value = 0-speed_test_value;
	test_mode_tick = us_ticker_read();
  }
  set_velocity(speed_test_value,0);

}

void OdevMotionControl::set_test_mode_speed(float speed){
	set_velocity(speed,0);
}
	

void OdevMotionControl::active() {
  prev_active_tick = us_ticker_read();
}
}
