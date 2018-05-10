#include "odev_sound.h"
#include "eeprom.h"
namespace Odev {

OdevSound::OdevSound(PinName tx, PinName rx) : pc(tx, rx, 9600) {
  state = 0;
  prev_update_tick = us_ticker_read();
}

uint8_t OdevSound::get_aud_type(void)
{
	return *(uint8_t *)(0x0801F800+2);
}

void OdevSound::play(uint8_t type, uint8_t index) {
  state = 1;
  uint8_t audio_buff[20] = {0x04, 0xFB, 0x10, 0x07, '/', '0', '0', '0', '1', '*', '/', '0', '0', '0', '1', '*', 'M', 'P', '3', 0x00};

  if(readEEPROMHalfWord(0)==1)
	audio_buff[5] = 0x31 + type / 1000;//”√”⁄”¢Œƒ”Ô“Ù
  else
	audio_buff[5] = 0x30 + type / 1000;  
  
  audio_buff[6] = 0x30 + type / 100;
  audio_buff[7] = 0x30 + type / 10;
  audio_buff[8] = 0x30 + type % 10;

  audio_buff[13] = 0x30 + index / 1000;
  audio_buff[14] = 0x30 + index / 100;
  audio_buff[13] = 0x30 + index / 10;
  audio_buff[14] = 0x30 + index % 10;

  audio_buff[19] = getSum(audio_buff, 19);
  send(audio_buff, 20);
}

void OdevSound::setVolume(uint8_t vol) {
  uint8_t volume1_buff[6] = {0x06, 0xF9, 0x02, 0x01, 0x16, 0x18};
  send(volume1_buff, 6);
}

void OdevSound::stop() {
  uint8_t audio_buff[5] = {0x04, 0xFB, 0x01, 0x03, 0x03};
  send(audio_buff, 5);
}

void OdevSound::send(uint8_t* buff, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    while (!pc.writeable()) {
    }
    pc.putc(buff[i]);
  }
}

uint8_t OdevSound::getSum(uint8_t* buff, uint8_t len) {
  uint8_t result = 0;
  for (int i = 0; i < len; i++) {
    result += buff[i];
  }

  return result;
}

void OdevSound::init() {
  setVolume(10);
}

void OdevSound::update() {
  if ((us_ticker_read() - prev_update_tick) > 100000) {
    prev_update_tick = us_ticker_read();

    if (state == 0) {
      stop();
      wait_ms(20);
      // setVolume(10);
    }
  }
}
}
