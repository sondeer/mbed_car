#include "odev_matrix_led.h"

#define SYS_DIS_CMD 0x80
#define SYS_EN_CMD 0X81
#define SYS_LED_ON_CMD 0X83
#define SYS_LED_OFF_CMD 0x82
#define SYS_RC_MODE0_CMD 0x98

namespace Odev {

OdevMatrixLed::OdevMatrixLed(PinName sda, PinName scl) : pc(sda, scl) {}

void OdevMatrixLed::init() {
  int address = 0xE4;
  char data[5] = {SYS_DIS_CMD, SYS_RC_MODE0_CMD, SYS_EN_CMD, SYS_LED_ON_CMD, 0xB1};
  pc.write(address, data, 5);
}

void OdevMatrixLed::display() {
  int address = 0xE4;
  if (us_ticker_read() - last_update_tick > 20000) {
    pc.write(address, (char*)&display_buffer, 25);
    last_update_tick = us_ticker_read();
  }
}

void OdevMatrixLed::clear() {
	for (int i = 0; i < 24; i++) {
    display_buffer.data[i] = 0;
  }
	pc.write(0xE4, (char*)&display_buffer, 25);
}

void OdevMatrixLed::setBuffer(uint8_t* buff) {
  display_buffer.data[3] = buff[0];
  display_buffer.data[5] = buff[1];
  display_buffer.data[10] = buff[2];
  display_buffer.data[4] = buff[3];
  display_buffer.data[6] = buff[4];
  display_buffer.data[9] = buff[5];
  display_buffer.data[7] = buff[6];
  display_buffer.data[8] = buff[7];

  display_buffer.data[0] = buff[8];
  display_buffer.data[2] = buff[9];
  display_buffer.data[21] = buff[10];
  display_buffer.data[1] = buff[11];
  display_buffer.data[13] = buff[12];
  display_buffer.data[20] = buff[13];
  display_buffer.data[14] = buff[14];
  display_buffer.data[15] = buff[15];

  display_buffer.data[16] = buff[16];
  display_buffer.data[11] = buff[17];
  display_buffer.data[17] = buff[18];
  display_buffer.data[12] = buff[19];
  display_buffer.data[22] = buff[20];
  display_buffer.data[18] = buff[21];
  display_buffer.data[23] = buff[22];
  display_buffer.data[19] = buff[23];

  for (int i = 0; i < 24; i++) {
    display_buffer.data[i] = ((display_buffer.data[i] >> 4) & 0x0f) | ((display_buffer.data[i] << 4) & 0xf0);
  }
}
}
