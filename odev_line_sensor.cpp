#include "odev_line_sensor.h"

namespace Odev {
OdevLineSensor::OdevLineSensor(PinName p0, PinName p1, PinName p2, PinName p3) : a0(p0), a1(p1), a2(p2), a3(p3) {
  last_measure_tick = us_ticker_read();
  sensors_state = 0;
  trig_a = 0x8000;//0x8000;//0x5000;
  trig_b = 0x8000;
  trig_c = 0x8000;
  trig_d = 0x8000;
}

uint8_t OdevLineSensor::getState() {
  return sensors_state;
}

void OdevLineSensor::update() {
  uint8_t temp = 0;
  if ((us_ticker_read() - last_measure_tick) > 5000) {
    adc_value_a = a0.read_u16();
    adc_value_b = a1.read_u16();
    adc_value_c = a2.read_u16();
    adc_value_d = a3.read_u16();
			
    if (adc_value_a > trig_a)
      temp |= 0x01;
    if (adc_value_b > trig_b)
      temp |= 0x02;
    if (adc_value_c > trig_c)
      temp |= 0x04;
    if (adc_value_d > trig_d)
      temp |= 0x08;

    sensors_state = temp;

    last_measure_tick = us_ticker_read();
  }
}

int8_t OdevLineSensor::getError() {
  switch (sensors_state) {
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

uint16_t OdevLineSensor::getLineSensorValue(int8_t senNum){
	if(senNum==0)
		return a0.read_u16();
	if(senNum==1)
        return a1.read_u16();
	if(senNum==2)
		return a2.read_u16();
	if(senNum==3)
		return a3.read_u16();
	return 0;
} 

void OdevLineSensor::setLineSensorValue(int8_t senNum,uint16_t senValue)
{
	if(senNum==0)
		trig_a = senValue;
	if(senNum==1)
        trig_b = senValue;
	if(senNum==2)
		trig_c = senValue;
	if(senNum==3)
		trig_d = senValue;
}

uint16_t OdevLineSensor::getLineSensorLevel(int8_t senNum)
{
	if(senNum==0)
		return trig_a;
	if(senNum==1)
        return trig_b;
	if(senNum==2)
		return trig_c;
	if(senNum==3)
		return trig_d;
	return 0;
}

}

