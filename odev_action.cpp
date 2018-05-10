/**
 * @file odev_action.cpp
 * @author gongpan
 * @version 1.1
 * @date 2017-06-22
 * @brief 自定义表情动作处理
 */

#include "odev_action.h"

namespace Odev {

const uint8_t face_empty[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t face_01_01[24] = {0x00, 0x72, 0x9C, 0xB8, 0xF8, 0xE8, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x33, 0x17, 0x1F, 0x1D, 0x0E, 0x00};
const uint8_t face_01_02[24] = {0x00, 0x02, 0x9C, 0xB8, 0xF8, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x33, 0x17, 0x1F, 0x1D, 0x00, 0x00};
const uint8_t face_01_03[24] = {0x00, 0x02, 0x04, 0x88, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x11, 0x1F, 0x00, 0x00, 0x00};

const uint8_t face_01_04[24] = {0x00, 0x02, 0x9C, 0xB8, 0xF8, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x33, 0x17, 0x1F, 0x1D, 0x00, 0x00};
const uint8_t face_01_05[24] = {0x00, 0x72, 0x9C, 0xB8, 0xF8, 0xE8, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x3F, 0x11, 0x00, 0x00};

const uint8_t face_02_01[24] = {0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x00};
const uint8_t face_02_02[24] = {0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x00};
const uint8_t face_02_03[24] = {0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x0E, 0x00};
const uint8_t face_02_04[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00};
const uint8_t* face_02_05 = face_02_03;
const uint8_t* face_02_06 = face_02_02;
const uint8_t* face_02_07 = face_02_01;

const uint8_t face_03_01[24] = {0x00, 0x00, 0xFE, 0x0E, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x07, 0x07, 0x02, 0x00, 0x00};
const uint8_t face_03_02[24] = {0x00, 0x00, 0xFE, 0x38, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x1C, 0x1C, 0x08, 0x00, 0x00};
const uint8_t face_03_03[24] = {0x00, 0x00, 0xFE, 0xE0, 0xE0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x70, 0x70, 0x20, 0x00, 0x00};
const uint8_t face_03_04[24] = {0x00, 0x00, 0x00, 0x00, 0x82, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x3E, 0x00, 0x00};
const uint8_t* face_03_05 = face_03_03;
const uint8_t* face_03_06 = face_03_02;
const uint8_t* face_03_07 = face_03_01;
const uint8_t* face_03_08 = face_03_04;

const uint8_t face_04_01[24] = {0x00, 0x44, 0xEE, 0xFE, 0xFE, 0x7C, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x77, 0x7F, 0x7F, 0x3E, 0x08, 0x00};
const uint8_t face_04_02[24] = {0x00, 0x00, 0x6C, 0x7C, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x3E, 0x1C, 0x08, 0x00, 0x00};
const uint8_t face_04_03[24] = {0x00, 0x00, 0x00, 0x28, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x08, 0x00, 0x00, 0x00};
const uint8_t* face_04_04 = face_04_02;
const uint8_t* face_04_05 = face_04_01;
const uint8_t* face_04_06 = face_empty;
const uint8_t* face_04_07 = face_04_01;
const uint8_t* face_04_08 = face_empty;
const uint8_t* face_04_09 = face_04_01;

const uint8_t face_05_01[24] = {0x00, 0x00, 0x04, 0x08, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x0F, 0x00, 0x00, 0x00};
const uint8_t face_05_02[24] = {0x00, 0x00, 0x04, 0x08, 0xF0, 0x00, 0x02, 0x00, 0x00, 0x00, 0x42, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x0F, 0x00, 0x40, 0x00};
const uint8_t face_05_03[24] = {0x00, 0x00, 0x04, 0x08, 0xF0, 0x00, 0x03, 0x01, 0x00, 0x00, 0x42, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x0F, 0x00, 0xC0, 0x80};

const uint8_t face_06_01[24] = {0x00, 0xF8, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00};
const uint8_t face_06_02[24] = {0x00, 0xF8, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00};
const uint8_t face_06_03[24] = {0x00, 0xF8, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00};
const uint8_t face_06_04[24] = {0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t face_06_05[24] = {0x00, 0xF8, 0xE0, 0x70, 0xE0, 0xC0, 0xE0, 0x00, 0x00, 0xC3, 0x00, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1F, 0x07, 0x03, 0x07, 0x0E, 0x07, 0x00};
const uint8_t face_06_06[24] = {0x00, 0xF8, 0xE0, 0xC0, 0xE0, 0x70, 0xE0, 0x00, 0x00, 0xC3, 0x00, 0x01, 0x00, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x07, 0x0E, 0x07, 0x03, 0x07, 0x00};
const uint8_t face_06_07[24] = {0x00, 0xF8, 0xE0, 0x70, 0xE0, 0xC0, 0xE0, 0x00, 0x00, 0xC3, 0x00, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1F, 0x07, 0x03, 0x07, 0x0E, 0x07, 0x00};
const uint8_t face_06_08[24] = {0x00, 0xF8, 0xE0, 0xC0, 0xE0, 0x70, 0xE0, 0x00, 0x00, 0xC3, 0x00, 0x01, 0x00, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x07, 0x0E, 0x07, 0x03, 0x07, 0x00};
const uint8_t face_06_09[24] = {0x00, 0xF8, 0xE0, 0x70, 0xE0, 0xC0, 0xE0, 0x00, 0x00, 0xC3, 0x00, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1F, 0x07, 0x03, 0x07, 0x0E, 0x07, 0x00};

const uint8_t face_07_01[24] = {0x00,0x04,0x08,0x10,0x10,0x08,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x10,0x08,0x08,0x10,0x20,0x00};
	//{0x00, 0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20};
const uint8_t face_07_02[24] = {0x00, 0x0C, 0x10, 0x20, 0x18, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x08, 0x04, 0x18, 0x00, 0x20, 0x00};
const uint8_t face_07_03[24] = {0x00, 0x0C, 0x10, 0x38, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x08, 0x1C, 0x20, 0x00, 0x00, 0x00};
const uint8_t face_07_04[24] = {0x00, 0x0C, 0x10, 0x20, 0x18, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x08, 0x04, 0x18, 0x00, 0x20, 0x00};
const uint8_t face_07_05[24] = {0x00,0x01,0x02,0x04,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x04,0x02,0x02,0x04,0x08,0x00};
	//{0x00, 0x01, 0x02, 0x04, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08};
const uint8_t face_07_06[24] = {0x00,0x10,0x20,0x40,0x40,0x20,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x20,0x40,0x80,0x00};
	//{0x00, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x20, 0x40, 0x80};
const uint8_t face_07_07[24] = {0x00,0x01,0x02,0x04,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x04,0x02,0x02,0x04,0x08,0x00};
	//{0x00, 0x01, 0x02, 0x04, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08};
const uint8_t face_07_08[24] = {0x00,0x10,0x20,0x40,0x40,0x20,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x20,0x40,0x80,0x00};
	//{0x00, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x20, 0x40, 0x80};
const uint8_t face_07_09[24] = {0x00, 0x72, 0x9C, 0xB8, 0xF8, 0xE8, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x33, 0x17, 0x1F, 0x1D, 0x0E, 0x00};

const uint8_t face_08_01[24] = {0x00, 0x08, 0x30, 0xF0, 0xF0, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x81, 0x00, 0x00, 0x00, 0x10, 0x0C, 0x0F, 0x0F, 0x07, 0x03, 0x00};
const uint8_t face_08_02[24] = {0x0E, 0x3C, 0xFC, 0xF8, 0xF8, 0xF0, 0xE0, 0x40, 0x00, 0x00, 0x81, 0xE7, 0xE7, 0xC3, 0x81, 0x00, 0x70, 0x3C, 0x3F, 0x1F, 0x1F, 0x0F, 0x07, 0x02};
const uint8_t* face_08_03 = face_08_01;
const uint8_t face_08_04[24] = {0x00, 0x08, 0x30, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x81, 0x00, 0x00, 0x00, 0x10, 0x0C, 0x0F, 0x07, 0x03, 0x01, 0x00};
const uint8_t face_08_05[24] = {0x00, 0x00, 0x00, 0x20, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x03, 0x01, 0x00, 0x00};
const uint8_t face_08_06[24] = {0x00, 0x00, 0x00, 0x28, 0xB0, 0xFC, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x36, 0x1F, 0x0E, 0x00};
const uint8_t face_08_07[24] = {0x00, 0x20, 0x20, 0x6C, 0xF8, 0xFC, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x81, 0x01, 0x83, 0x00, 0x00, 0x00, 0x04, 0x04, 0x2D, 0x3F, 0x7F, 0x1F, 0x0E};
const uint8_t face_08_08[24] = {0x00, 0x40, 0x40, 0x68, 0xF8, 0xFC, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x03, 0x01, 0x83, 0x01, 0x00, 0x00, 0x08, 0x08, 0x6D, 0x3F, 0x7F, 0x1E, 0x0C};
const uint8_t* face_08_09 = face_08_07;

const uint8_t face_09_01[24] = {0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x00};
const uint8_t face_09_02[24] = {0x00, 0x00, 0x38, 0x7C, 0xC6, 0x82, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x3E, 0x63, 0x41, 0x41, 0x00};
const uint8_t face_09_03[24] = {0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x00, 0x00};
const uint8_t face_09_04[24] = {0x00, 0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41};
const uint8_t face_09_05[24] = {0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x00, 0x00};
const uint8_t face_09_06[24] = {0x00, 0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41};

const uint8_t face_power_low_01[24] = {0x00, 0x00, 0x00, 0x04, 0x08, 0xF0, 0x03, 0x01, 0x00, 0x00, 0x00, 0x42, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x0F, 0xC0, 0x80};
const uint8_t face_power_low_02[24] = {0x00, 0xFC, 0x70, 0x70, 0x70, 0x70, 0x70, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x00};
const uint8_t face_power_low_03[24] = {0x00, 0x00, 0x00, 0x04, 0x08, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x0F, 0x00, 0x00};
const uint8_t face_power_low_04[24] = {0x00, 0x00, 0x00, 0x04, 0x08, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x42, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x0F, 0x40, 0x00};
const uint8_t* face_power_low_05 = face_power_low_01;
const uint8_t* face_power_low_06 = face_power_low_02;

const uint8_t face_power_off_01[24] = {0x00, 0x44, 0xEE, 0xFE, 0xFE, 0x7C, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x36, 0x41, 0x41, 0x41, 0x41, 0x00};
const uint8_t face_power_off_02[24] = {0x00,0x04,0x08,0x10,0x10,0x08,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x10,0x08,0x08,0x10,0x20,0x00};
	//{0x00, 0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20};
const uint8_t face_power_off_03[24] = {0x00, 0x00, 0x00, 0x00, 0x02, 0x84, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x21, 0x1E, 0x00};

//const uint8_t face_idle_01[24] = {0x00, 0x00, 0x60, 0xF0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0F, 0x06, 0x00, 0x00, 0x00};
//const uint8_t face_idle_02[24] = {0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00};
const uint8_t face_idle_01[24] = {0x00,0xF8,0x04,0x04,0x1C,0x1C,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x20,0x20,0x23,0x23,0x1F,0x00};
const uint8_t face_idle_02[24] = {0x00,0xF8,0x04,0x04,0x64,0x64,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x20,0x20,0x2C,0x2C,0x1F,0x00};
const uint8_t face_idle_03[24] = {0x00,0xF8,0x04,0x04,0xC4,0xC4,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x20,0x20,0x38,0x38,0x1F,0x00};
const uint8_t face_idle_04[24] = {0x00,0xF8,0x04,0x1C,0x1C,0x04,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x20,0x23,0x23,0x20,0x1F,0x00};
const uint8_t face_idle_05[24] = {0x00,0xF8,0x34,0x34,0x04,0x04,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x2C,0x20,0x20,0x20,0x1F,0x00};
const uint8_t face_idle_06[24] = {0x00,0xF8,0xC4,0xC4,0x04,0x04,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x38,0x38,0x20,0x20,0x1F,0x00};
const uint8_t face_idle_07[24] = {0x00,0x00,0xC4,0xC4,0x04,0x04,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x00,0x38,0x38,0x20,0x20,0x1F,0x00};
const uint8_t face_idle_08[24] = {0x00,0x00,0x04,0xC4,0x04,0x04,0xF8,0x00,0x00,0x00,0x80,0x81,0x81,0x81,0x00,0x00,0x00,0x00,0x00,0x38,0x20,0x20,0x1F,0x00};
const uint8_t face_idle_09[24] = {0x00,0x00,0x00,0x00,0x04,0x04,0xF8,0x00,0x00,0x00,0x00,0x00,0x81,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x1F,0x00};
const uint8_t face_idle_10[24] = {0x00,0x00,0x00,0xC4,0x04,0x04,0xF8,0x00,0x00,0x00,0x00,0x81,0x81,0x81,0x00,0x00,0x00,0x00,0x00,0x38,0x20,0x20,0x1F,0x00};
const uint8_t face_idle_11[24] = {0x00,0x00,0xC4,0xC4,0x04,0x04,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x00,0x38,0x38,0x20,0x20,0x1F,0x00};
const uint8_t face_idle_12[24] = {0x00,0x00,0x00,0xC4,0x04,0x04,0xF8,0x00,0x00,0x00,0x00,0x81,0x81,0x81,0x00,0x00,0x00,0x00,0x00,0x38,0x20,0x20,0x1F,0x00};
const uint8_t face_idle_13[24] = {0x00,0x00,0x00,0xC4,0x04,0x04,0xF8,0x00,0x00,0x00,0x00,0x81,0x81,0x81,0x00,0x00,0x00,0x00,0x00,0x38,0x20,0x20,0x1F,0x00};
const uint8_t face_idle_14[24] = {0x00,0x00,0xC4,0xC4,0x04,0x04,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x00,0x38,0x38,0x20,0x20,0x1F,0x00};
const uint8_t face_idle_15[24] = {0x00,0xF8,0xC4,0xC4,0x04,0x04,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x38,0x38,0x20,0x20,0x1F,0x00};
const uint8_t face_idle_16[24] = {0x00,0xF8,0x04,0x64,0x64,0x04,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x20,0x2C,0x2C,0x20,0x1F,0x00};

const uint8_t face_charge_01[24] = {0x00,0xF8,0x04,0x04,0x04,0x04,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00};
const uint8_t face_charge_02[24] = {0x00,0xF8,0x04,0x04,0x04,0xFC,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x3F,0x1F,0x00};
const uint8_t face_charge_03[24] = {0x00,0xF8,0x04,0x04,0xFC,0xFC,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x20,0x20,0x3F,0x3F,0x1F,0x00};
const uint8_t face_charge_04[24] = {0x00,0xF8,0x04,0xFC,0xFC,0xFC,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x20,0x3F,0x3F,0x3F,0x1F,0x00};
const uint8_t face_charge_05[24] = {0x00,0xF8,0xFC,0xFC,0xFC,0xFC,0xF8,0x00,0x00,0x00,0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x1F,0x3F,0x3F,0x3F,0x3F,0x1F,0x00};
			
const uint8_t sound_count[12] = {20, 18, 17, 9, 13, 13, 25, 17, 18, 17, 23, 6};

OdevAction::OdevAction() {
  prev_active_tick = us_ticker_read();
  idle_action_repeat_tick = us_ticker_read();
  prev_obs_action_tick = us_ticker_read();
  prev_update_tick = us_ticker_read();

  prev_roam_obs_action_tick = us_ticker_read();
  prev_roam_no_obs_action_tick = us_ticker_read();

  prev_follow_obs_action_tick = us_ticker_read();
  prev_follow_no_obs_action_tick = us_ticker_read();

  prev_line_mode_action_tick = us_ticker_read();
  prev_draw_mode_action_tick = us_ticker_read();

  action_delay_tick = 0;
  action_idle_tick = 0;
  
  key_action_speed = 50;
  idle = 0;
  avoid_obs = 0;
  silent_mode = 0;
  
  avoid_obs_fliter_tick = 0;
  action_power_state = action_power_off;

  set_manual_face(face_01_01);
}

/**
 * 调用点阵LED驱动
 * @param buff 点阵LED数据
 */
void OdevAction::set_led_buff(const uint8_t* buff) {
  if (set_led_matrix_callback) {
    set_led_matrix_callback(buff);
  }
}

/**
 * 调用电机驱动
 * @param left 左轮速度
 * @param right 右轮速度
 */
void OdevAction::set_speed(int8_t left, int8_t right) {
  if (set_motor_speed_callback) {
    set_motor_speed_callback(left, right);
  }
}

/**
 * 调用声音驱动
 * @param type 类型文件夹
 * @param index 文件名编号
 */
void OdevAction::set_sound(uint8_t type, uint8_t index) {
  if (set_sound_callback && (!silent_mode)) {
    set_sound_callback(type, index);
  }
}

/**
 * 添加点阵表情
 * @param buff 表情动作
 * @param delay 持续延时
 */
void OdevAction::add_face(const uint8_t* buff, uint32_t delay) {
  struct odev_action action;

  action.led_buffer = buff;
  action.delay = delay;
  action.sound_index_h = 0;
  action.sound_index_l = 0;
  action.motor_left = 0;
  action.motor_right = 0;
  action.type_mask = 0x01;
  action_buffer.push(action);
}

/// 清空表情动作缓冲区并停止两轮
void OdevAction::clear_action_buffer_and_stop_motor(void) {
  action_buffer.reset();
  set_speed(0, 0);
  action_delay_tick = 0;
}

/// 添加轮子动作
void OdevAction::add_motor(int8_t left, int8_t right, uint32_t delay) {
  struct odev_action action;

  action.led_buffer = 0;
  action.delay = delay;
  action.sound_index_h = 0;
  action.sound_index_l = 0;
  action.motor_left = left;
  action.motor_right = right;
  action.type_mask = 0x02;
  action_buffer.push(action);
}

/// 添加语音动作
void OdevAction::add_sound(uint8_t a, uint8_t b, uint32_t delay) {
  struct odev_action action;

  action.led_buffer = 0;
  action.delay = delay;
  action.sound_index_h = a;
  action.sound_index_l = b;
  action.motor_left = 0;
  action.motor_right = 0;
  action.type_mask = 0x04;
  action_buffer.push(action);
}

/// 添加所有类型的动作（点阵，轮子，语音）
void OdevAction::add_all_action_type(const uint8_t* buff, int8_t motor_left, int8_t motor_right, uint8_t a, uint8_t b, uint32_t delay) {
  struct odev_action action;

  action.led_buffer = buff;
  action.delay = delay;
  action.sound_index_h = a;
  action.sound_index_l = b;
  action.motor_left = motor_left;
  action.motor_right = motor_right;
  action.type_mask = 0x07;
  action_buffer.push(action);
}

/// 9种自定义表情动画
void OdevAction::add_face_action(uint8_t index, uint32_t delay) {
  switch (index) {
    case 1:
      add_face(face_01_01, 100);
      add_face(face_01_02, 100);
      add_face(face_01_03, 100);
      add_face(face_01_04, 100);
      add_face(face_01_05, 100);
      add_face(face_01_05, delay);
      break;
    case 2:
      add_face(face_02_01, 50);
      add_face(face_02_02, 50);
      add_face(face_02_03, 50);
      add_face(face_02_04, 50);
      add_face(face_02_05, 50);
      add_face(face_02_06, 50);
      add_face(face_02_07, 50);
      add_face(face_02_07, delay);
      break;
    case 3:
      add_face(face_03_01, 100);
      add_face(face_03_02, 100);
      add_face(face_03_03, 100);
      add_face(face_03_04, 100);
      add_face(face_03_05, 100);
      add_face(face_03_06, 100);
      add_face(face_03_07, 100);
      add_face(face_03_08, 100);
      add_face(face_03_08, delay);
      break;
    case 4:
      add_face(face_04_01, 100);
      add_face(face_04_02, 100);
      add_face(face_04_03, 100);
      add_face(face_04_04, 100);
      add_face(face_04_05, 100);
      add_face(face_04_06, 100);
      add_face(face_04_07, 100);
      add_face(face_04_08, 100);
      add_face(face_04_08, delay);
      break;
    case 5:
      add_face(face_05_01, 100);
      add_face(face_05_02, 100);
      add_face(face_05_03, 100);
      add_face(face_05_03, delay);
      break;
    case 6:
      add_face(face_06_01, 100);
      add_face(face_06_02, 100);
      add_face(face_06_03, 100);
      add_face(face_06_04, 100);
      add_face(face_06_05, 100);
      add_face(face_06_06, 100);
      add_face(face_06_07, 100);
      add_face(face_06_08, 100);
      add_face(face_06_09, 100);
      add_face(face_06_09, delay);
      break;
    case 7:
      add_face(face_07_01, 100);
      add_face(face_07_02, 100);
      add_face(face_07_03, 100);
      add_face(face_07_04, 100);
      add_face(face_07_05, 100);
      add_face(face_07_06, 100);
      add_face(face_07_07, 100);
      add_face(face_07_08, 100);
      add_face(face_07_09, 100);
      add_face(face_07_09, delay);
      break;
    case 8:
      add_face(face_08_01, 100);
      add_face(face_08_02, 100);
      add_face(face_08_03, 100);
      add_face(face_08_04, 100);
      add_face(face_08_05, 100);
      add_face(face_08_06, 100);
      add_face(face_08_07, 100);
      add_face(face_08_08, 100);
      add_face(face_08_09, 100);
      add_face(face_08_09, delay);
      break;
    case 9:
      add_face(face_09_01, 100);
      add_face(face_09_02, 100);
      add_face(face_09_03, 100);
      add_face(face_09_04, 100);
      add_face(face_09_05, 100);
      add_face(face_09_06, 100);
      add_face(face_09_06, delay);
      break;
    default:
      break;
  }
}

/// APP设置的表情
void OdevAction::set_manual_face(const uint8_t* buff) {
  memcpy(face_manual, buff, 24);
}

//设置eye转动的表情
void OdevAction::set_eye_face(void) {
	add_face(face_idle_01, 500);
	add_face(face_idle_02, 200);
	add_face(face_idle_03, 1000);
	add_face(face_idle_04, 1000);
	add_face(face_idle_05, 200);
	add_face(face_idle_06, 1000);
	add_face(face_idle_07, 100);
	add_face(face_idle_08, 100);
	add_face(face_idle_09, 100);
	add_face(face_idle_10, 100);
	add_face(face_idle_11, 200);
	add_face(face_idle_12, 100);
	add_face(face_idle_13, 100);
	add_face(face_idle_14, 100);
	add_face(face_idle_15, 1000);
	add_face(face_idle_16, 100);
}

/// 循环处理函数
void OdevAction::update(uint8_t distance, uint8_t mode) {
  uint8_t random_value;

  struct odev_action action;

  if ((us_ticker_read() - prev_update_tick) > 1000) {
    prev_update_tick = us_ticker_read();

    //距离小于50cm接近障碍物
    if (distance < 50) 
	{
		if(avoid_obs==0xff)
			avoid_obs=1;
		avoid_obs++;
    } else {
      avoid_obs = 0;
    }
	
	if(avoid_obs!=0)
	{
		avoid_obs_fliter_tick++;
	}
	else
		avoid_obs_fliter_tick = 0;

    if (action_delay_tick == 0) {
      if (action_buffer.pop(action)) {
        if (((action.type_mask & 0x01) > 0) && (action.led_buffer != NULL)) {
          set_led_buff(action.led_buffer);
        }

        if ((action.type_mask & 0x02) > 0) {
          set_speed(action.motor_left, action.motor_right);
        }

        if (((action.type_mask & 0x04) > 0) && (action.sound_index_h != 0 || action.sound_index_l != 0)) {
          set_sound(action.sound_index_h, action.sound_index_l);
        }

        action_delay_tick = action.delay;
        action_idle_tick = 0;
      } else {
        // 表情缓冲区中没有待播放的表情动作
        if (idle == true) {
			set_eye_face();
        } else {          
//          if((action_delay_tick == 0)&&(action_power_state == action_power_on)) 
//			  set_eye_face();
//		  else
			  add_face(face_manual, 200);
        }
      }

    } else {
      action_delay_tick--;
    }
  }

  if ((idle == false) && ((us_ticker_read() - prev_active_tick) > 60000000)) {
    idle = true;
    // idle_action();
    prev_active_tick = us_ticker_read();
    // idle_action_repeat_tick = us_ticker_read();
  }
  if(action_power_state != action_power_on)
	  return;
  switch (mode & 0x7f) {
    case 0:  // 普通模式
      if(idle == true)
	  {
		 if(((us_ticker_read() - idle_action_repeat_tick) > 30000000)
			  &&((avoid_obs) && ((us_ticker_read() - prev_obs_action_tick) > 10000000)))
		 {
		   srand(us_ticker_read());
		   random_value = rand();
		   switch (random_value%2)
		   {
			 case 0:
				 idle_action();
				 idle_action_repeat_tick = us_ticker_read();
				 //避免和避障的语音相互覆盖
				 prev_obs_action_tick = us_ticker_read(); 
				 break;
			 case 1:
				 obs_action();
				 prev_obs_action_tick = us_ticker_read();
				//避免和空闲时候语音覆盖
				 idle_action_repeat_tick = us_ticker_read();
				 break;
			 default:
				 break;
					 
		    }			  
		  }
		  else
		  {
			if((us_ticker_read() - idle_action_repeat_tick) > 30000000) {
				idle_action();
				idle_action_repeat_tick = us_ticker_read();	
				prev_obs_action_tick = us_ticker_read();//-10000000;				
			}

			if((avoid_obs) && ((us_ticker_read() - prev_obs_action_tick) > 10000000)) {
				obs_action();
				prev_obs_action_tick = us_ticker_read();				
			}
		 }
	  }
      break;
    case 1:  // 漫游模式
      if ((avoid_obs) && ((us_ticker_read() - prev_roam_obs_action_tick) > 10000000)) {
        roam_obs_action();
        update_mode_action_ticks();
      }

      if ((!avoid_obs) && ((us_ticker_read() - prev_roam_no_obs_action_tick) > 30000000)) {
        roam_no_obs_action();
        update_mode_action_ticks();
      }
      break;
    case 2:  // 巡线模式
      if ((us_ticker_read() - prev_line_mode_action_tick) > (rand() % 30 + 30) * 1000000) {
        line_mode_action();
        update_mode_action_ticks();
      }
      break;
    case 3:  // 轨迹模式
      if ((us_ticker_read() - prev_draw_mode_action_tick) > 10000000) {
        draw_mode_action();
        update_mode_action_ticks();
      }
      break;
    case 4:  // 跟随模式
      if (((avoid_obs) && ((us_ticker_read() - prev_follow_obs_action_tick) > 10000000))
		  ||((avoid_obs_fliter_tick>=800)&&(avoid_obs_fliter_tick<2000)) )
	  {
			follow_obs_action();
			update_mode_action_ticks();
			avoid_obs_fliter_tick = 3000;
      }

      if ((!avoid_obs) && ((us_ticker_read() - prev_follow_no_obs_action_tick) > (rand() % 20 + 20) * 1000000)) {
        follow_no_obs_action();
        update_mode_action_ticks();
      }
      break;
    default:
      break;
  }
  
}

void OdevAction::update_mode_action_ticks() {
  prev_roam_obs_action_tick = us_ticker_read();
  prev_roam_no_obs_action_tick = us_ticker_read();
  prev_follow_obs_action_tick = us_ticker_read();
  prev_follow_no_obs_action_tick = us_ticker_read();
  prev_line_mode_action_tick = us_ticker_read();
  prev_draw_mode_action_tick = us_ticker_read();
}

void OdevAction::update_ticks() {
  struct odev_action action;

  if (action_delay_tick == 0) {
    if (action_buffer.pop(action)) {
      if (((action.type_mask & 0x01) > 0) && (action.led_buffer != NULL)) {
        set_led_buff(action.led_buffer);
      }

      if ((action.type_mask & 0x02) > 0) {
        set_speed(action.motor_left, action.motor_right);
      }

      if (((action.type_mask & 0x04) > 0) && (action.sound_index_h != 0 || action.sound_index_l != 0)) {
        set_sound(action.sound_index_h, action.sound_index_l);
      }

      action_delay_tick = action.delay;
    }

  } else {
    action_delay_tick--;
  }
}

/// 开机表情动作
void OdevAction::power_on_action() {
  uint8_t random_value = 2;
  
  action_power_state = action_power_on;
  srand(us_ticker_read());
  random_value = rand();
  switch (random_value % 3) {
    case 0:
      add_sound(7, 18, 0);
      add_face_action(1, 3000);
      break;

    case 1:
      add_sound(8, 2, 0);
      add_face_action(2, 3000);
      break;

    case 2:
      add_sound(6, 3, 0);
      add_face_action(9, 3000);
      break;
  }
  idle = true;
}

void OdevAction::set_idle_speed(uint8_t speed)
{
	key_action_speed = speed;
}
/// 空闲表情状态
void OdevAction::idle_action() {
  uint8_t random_value = 0,random_value1 = 0;
  uint8_t action_speed = 50;
  static uint8_t lastrandom_value = 0;
  
  if(key_action_state==true)
	  action_speed = key_action_speed;
  else
	  action_speed = 50;
  
  srand(us_ticker_read());
  random_value = rand();
  if(random_value%7==lastrandom_value)
  {	  	
	  random_value ++;	  	  
  }
  lastrandom_value =random_value%7;
   
  switch (random_value % 7) {
	case 0:
	  srand(us_ticker_read());
	  random_value1 = rand();
	  switch(random_value1 % 7)
	  {
		case 0:
		  add_all_action_type(face_04_01, 0, 0, 1, 1, 1000);
		  add_all_action_type(face_02_01, action_speed, 0, 0, 0, 1000);
		  add_all_action_type(face_09_01, action_speed, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_09_01, 0, 0, 1, 11, 2000);
		  break;
		case 1:
		  add_all_action_type(face_04_01, 0, 0, 1, 1, 1000);
		  add_all_action_type(face_02_01, 0, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_09_01, action_speed, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_09_01, 0, 0, 1, 11, 2000);
		  break;
		case 2:
		  add_all_action_type(face_04_01, 0, 0, 1, 1, 1000);
		  add_all_action_type(face_02_01, 0, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_02_01, action_speed, 0, 0, 0, 2000);
		  add_all_action_type(face_09_01, action_speed, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_09_01, 0, 0, 1, 11, 2000);
		  break;
		case 3:
		  add_all_action_type(face_04_01, 0, 0, 1, 1, 1000);
		  add_all_action_type(face_02_01, action_speed, 0, 0, 0, 1000);
		  add_all_action_type(face_02_01, 0, -action_speed, 0, 0, 2000);
		  add_all_action_type(face_09_01, action_speed, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_09_01, 0, 0, 1, 11, 2000);
		  break;
		case 4:
		  add_all_action_type(face_04_01, 0, 0, 1, 1, 1000);
		  add_all_action_type(face_02_01, 0, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_02_01, action_speed, 0, 0, 0, 2000);
		  add_all_action_type(face_02_01, 0, -action_speed, 0, 0, 2000);
		  add_all_action_type(face_09_01, action_speed, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_09_01, 0, 0, 1, 11, 2000);
		  break;
		case 5:
		  add_all_action_type(face_04_01, 0, 0, 1, 1, 1000);
		  add_all_action_type(face_02_01, action_speed, 0, 0, 0, 1000);
		  add_all_action_type(face_02_01, 0, -action_speed, 0, 0, 2000);
		  add_all_action_type(face_02_01, action_speed, 0, 0, 0, 2000);
		  add_all_action_type(face_09_01, action_speed, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_09_01, 0, 0, 1, 11, 2000);
		  break;
		case 6:
		  add_all_action_type(face_02_01, action_speed, 0, 0, 0, 1000);
		  add_all_action_type(face_02_01, 0, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_02_01, action_speed, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_02_01, -action_speed, action_speed, 0, 0, 2000);

		  add_all_action_type(face_02_01, action_speed, 0, 0, 0, 1000);
		  add_all_action_type(face_02_01, 0, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_02_01, action_speed, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_02_01, -action_speed, action_speed, 0, 0, 2000);

		  add_all_action_type(face_02_01, action_speed, 0, 0, 0, 1000);
		  add_all_action_type(face_02_01, 0, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_02_01, action_speed, -action_speed, 0, 0, 1000);
		  add_all_action_type(face_02_01, -action_speed, action_speed, 0, 0, 2000);

		  add_all_action_type(face_09_01, 0, 0, 1, 2, 2000);
		  break;
		default:
		  break;
		}
	  break;
    case 1:
      add_all_action_type(face_02_01, action_speed, 0, 6, 14, 1000);
      add_all_action_type(face_02_01, 0, -action_speed, 0, 0, 1000);
      add_all_action_type(face_09_01, 0, 0, 0, 0, 1000);
      break;
    case 2:
      add_all_action_type(face_09_01, action_speed, -(action_speed*2/3), 2, 18, 500);
      add_all_action_type(face_01_01, action_speed, -(action_speed*2/3), 0, 0, 500);
	  add_all_action_type(face_01_02, action_speed, -(action_speed*2/3), 0, 0, 500);
	  add_all_action_type(face_01_03, action_speed, -(action_speed*2/3), 0, 0, 500);
	  add_all_action_type(face_01_04, action_speed, -(action_speed*2/3), 0, 0, 500);
	  add_all_action_type(face_01_01, 0, 0, 0, 0, 1000);
      break;
//    case 3:
//      add_all_action_type(face_04_01, 50, 50, 7, 21, 2000);
//      add_all_action_type(face_04_01, 0, 0, 0, 0, 2000);
//      break;
	case 3:
	  add_all_action_type(face_04_01, action_speed, action_speed, 7, 21,400);
	  add_all_action_type(face_04_02, action_speed, action_speed, 0, 0, 400);
	  add_all_action_type(face_04_03, action_speed, action_speed, 0, 0, 400);
	  add_all_action_type(face_04_04, action_speed, action_speed, 0, 0, 400);
	  add_all_action_type(face_04_05, action_speed, action_speed, 0, 0, 400);
	  add_all_action_type(face_04_06, action_speed, action_speed, 0, 0, 400);
	  add_all_action_type(face_04_07, action_speed, action_speed, 0, 0, 400);
	  add_all_action_type(face_04_08, 00, 00, 0, 0, 400);
	  add_all_action_type(face_04_09, 00, 00, 0, 0, 500);
	  break;
	case 4:
	  add_all_action_type(face_09_01, action_speed, -action_speed, 2, 5, 400);
	  add_all_action_type(face_09_02, -action_speed, action_speed, 0, 0, 400);
	  add_all_action_type(face_09_03, action_speed, -action_speed, 0, 0, 400);
	  add_all_action_type(face_09_04, -action_speed, action_speed, 0, 0, 400);
	  add_all_action_type(face_09_05, action_speed, -action_speed, 0, 0, 400);
	  add_all_action_type(face_09_06, 0, 0, 0, 0, 800);
	  break;
	case 5:
	  add_all_action_type(face_08_01, action_speed, -action_speed, 11, 22,1000);
	  add_all_action_type(face_08_02, action_speed, -action_speed, 0, 0, 150);
	  add_all_action_type(face_08_03, action_speed, -action_speed, 0, 0, 150);
	  add_all_action_type(face_08_04, action_speed, -action_speed, 0, 0, 150);
	  add_all_action_type(face_08_05, action_speed, -action_speed, 0, 0, 150);
	  add_all_action_type(face_08_06, action_speed, -action_speed, 0, 0, 150);
	  add_all_action_type(face_08_07, action_speed, -action_speed, 0, 0, 150);
	  add_all_action_type(face_08_08, action_speed, -action_speed, 0, 0, 150);
	  add_all_action_type(face_08_09, 0, 0, 0, 0, 2000);
	  break;
	case 6:
	  add_all_action_type(face_02_01, 0, 0, 4, 1, 150);
	  add_all_action_type(face_02_02, 0, 0, 0, 0, 150);
	  add_all_action_type(face_02_03, 0, 0, 0, 0, 150);
	  add_all_action_type(face_02_04, 0, 0, 0, 0, 150);
	  add_all_action_type(face_02_05, 0, 0, 0, 0, 150);
	  add_all_action_type(face_02_06, 0, 0, 0, 0, 150);
	  add_all_action_type(face_02_07, 0, 0, 0, 0, 1000);
	  break;
    default:
      break;
  }
}

/// 关机表情动作
void OdevAction::power_off_action() {
  uint8_t random_value = 0;
	
  action_power_state = action_power_off;
  srand(us_ticker_read());
  random_value = rand();

  set_manual_face(face_empty);

  clear_action_buffer_and_stop_motor();

  switch (random_value % 3) {
    case 0:
      add_sound(7, 24, 0);
      add_face(face_power_off_01, 1000);
      add_face(face_empty, 2000);
      break;

    case 1:
      add_sound(7, 25, 0);
      add_face(face_power_off_02, 1000);
      add_face(face_empty, 2000);
      break;

    case 2:
      add_sound(7, 23, 0);
      add_face(face_power_off_03, 1000);
      add_face(face_empty, 2000);
      break;
  }
}

void OdevAction::test() {
  active();

  add_sound(13, 19, 60);
  add_sound(13, 19, 60);
  add_sound(13, 23, 60);
  add_sound(13, 23, 60);
  add_sound(13, 24, 60);
  add_sound(13, 24, 60);
  add_sound(13, 23, 60);
}

/// 按键表情动作
void OdevAction::key_action() {
  //if (idle != 0) 
  //if()	
  {
	key_action_state = true;
	clear_action_buffer_and_stop_motor();
    idle_action();
	//避免按键的语音被覆盖  
	idle_action_repeat_tick = us_ticker_read();
	prev_obs_action_tick = us_ticker_read(); 
	update_mode_action_ticks();
	key_action_state = false;	  
  }
}

/// 遇见障碍表情动作
void OdevAction::obs_action() {
  uint8_t random_value = 0;
  static uint8_t lastrandom_value = 0;
	
  srand(us_ticker_read());
  random_value = rand();
	
  if(random_value%3==lastrandom_value)
  {	  	
	  random_value ++;	  	  
  }
  lastrandom_value =random_value%3;
   

  clear_action_buffer_and_stop_motor();

  switch (random_value % 3) {
    case 0:
      add_sound(7, 20, 0);
      add_face_action(3, 1000);
      break;

    case 1:
      add_sound(7, 19, 0);
      add_face_action(7, 1000);
      break;

    case 2:
      add_sound(7, 18, 0);
      add_face_action(9, 1000);
      break;
  }
}

/// 电压低表情动作
void OdevAction::power_low_action() {
  clear_action_buffer_and_stop_motor();
  add_all_action_type(face_power_low_01, 0, 0, 7, 22, 100);
  add_face(face_power_low_02, 100);
  add_face(face_power_low_03, 100);
  add_face(face_power_low_04, 100);
  add_face(face_power_low_05, 100);
  add_face(face_power_low_06, 3000);
	
  //避免和避障的语音相互覆盖
  prev_obs_action_tick = us_ticker_read(); 
}

/// 漫游模式有障碍物表情动作
void OdevAction::roam_obs_action() {
  uint8_t random_value = 0;
  static uint8_t lastrandom_value = 0;
	
  srand(us_ticker_read());
  random_value = rand();
	
  if(random_value%4==lastrandom_value)
  {	  	
	  random_value ++;	  	  
  }
  lastrandom_value =random_value%4;

  switch (random_value % 4) {
    case 0:
      add_sound(3, 4, 0);
      add_face_action(7, 100);
      break;
    case 1:
      add_sound(3, 8, 0);
      add_face_action(2, 100);
      break;
    case 2:
      add_sound(3, 6, 0);
      add_face_action(7, 100);
      break;
    case 3:
      add_sound(10, 9, 0);
      add_face_action(2, 100);
      break;
    default:
      break;
  }
}

/// 漫游模式无障碍表情动作
void OdevAction::roam_no_obs_action() {
  uint8_t random_value = 0;
  static uint8_t lastrandom_value = 0;
	
  srand(us_ticker_read());
  random_value = rand();

  if(random_value%8==lastrandom_value)
  {	  	
	  random_value ++;	  	  
  }
  lastrandom_value =random_value%8;

  switch (random_value % 8) {
    case 0:
      add_sound(10, 1, 0);
      add_face_action(8, 3000);
      add_sound(10, 1, 0);
      add_face_action(8, 3000);
      break;
    case 1:
      add_sound(10, 7, 0);
      add_face_action(8, 3000);
      add_sound(10, 7, 0);
      add_face_action(8, 3000);
      break;
    case 2:
      add_sound(10, 6, 0);
      add_face_action(2, 3000);
      add_sound(10, 6, 0);
      add_face_action(2, 3000);
      break;
    case 3:
      add_sound(10, 10, 0);
      add_face_action(2, 3000);
      add_sound(10, 10, 0);
      add_face_action(2, 3000);
      break;
    case 4:
      add_sound(10, 16, 0);
      add_face_action(8, 3000);
      add_sound(10, 16, 0);
      add_face_action(8, 3000);
      break;
    case 5:
      add_sound(10, 12, 0);
      add_face_action(2, 3000);
      add_sound(10, 12, 0);
      add_face_action(2, 3000);
      break;
    case 6:
      add_sound(10, 11, 0);
      add_face_action(2, 3000);
      add_sound(10, 11, 0);
      add_face_action(2, 3000);
      break;
    case 7:
      add_sound(2, 18, 0);
      add_face_action(1, 3000);
      add_sound(2, 18, 0);
      add_face_action(1, 3000);
      break;
    default:
      break;
  }
}

/// 引导模式有障碍表情动作
void OdevAction::follow_obs_action() {
  uint8_t random_value = 0;
  static uint8_t lastrandom_value = 0;	
	
  srand(us_ticker_read());
  random_value = rand();

  if(random_value%5==lastrandom_value)
  {	  	
	  random_value ++;	  	  
  }
  lastrandom_value =random_value%5;

  switch (random_value % 5) {
    case 0:
      add_sound(6, 13, 0);
      add_face_action(9, 0);
      break;
    case 1:
      add_sound(7, 21, 0);
      add_face_action(4, 0);
      break;
    case 2:
      add_sound(7, 8, 0);
      add_face_action(9, 0);
      break;
    case 3:
      add_sound(2, 8, 0);
      add_face_action(8, 0);
      break;
    case 4:
      add_sound(7, 14, 0);
      add_face_action(1, 0);
      break;
    default:
      break;
  }
}

/// 引导模式无障碍表情动作
void OdevAction::follow_no_obs_action() {
  uint8_t random_value = 0;
  static uint8_t lastrandom_value = 0;
	
  srand(us_ticker_read());
  random_value = rand();

  if(random_value%4==lastrandom_value)
  {	  	
	  random_value ++;	  	  
  }
  lastrandom_value =random_value%4;

  switch (random_value % 4) {
    case 0:
      add_sound(7, 19, 0);
      add_face_action(1, 500);
      break;
    case 1:
      add_sound(3, 17, 0);
      add_face_action(2, 500);
      break;
    case 2:
      add_sound(7, 16, 0);
      add_face_action(5, 500);
      break;
    case 3:
      add_sound(10, 9, 0);
      add_face_action(7, 500);
      break;
    default:
      break;
  }
}

/// 巡线模式表情动作
void OdevAction::line_mode_action() {
  static uint8_t lastrandom_value = 0;
  uint8_t random_value = 0;
	
  srand(us_ticker_read());
  random_value = rand();
	
  if(random_value%5==lastrandom_value)
  {	  	
	  random_value ++;	  	  
  }
  lastrandom_value =random_value%5;

  switch (random_value % 5) {
    case 0:
      add_sound(1, 18, 0);
      add_face_action(9, 0);
      break;
    case 1:
      add_sound(2, 18, 0);
      add_face_action(4, 0);
      break;
    case 2:
      add_sound(12, 2, 0);
      add_face_action(9, 0);
      break;
    case 3:
      add_sound(6, 7, 0);
      add_face_action(8, 0);
      break;
    case 4:
      add_sound(6, 14, 0);
      add_face_action(1, 0);
      break;
    default:
      break;
  }
}

/// 轨迹模式表情动作
void OdevAction::draw_mode_action() {
  uint8_t random_value = 0;
  //uint8_t sound_type = 0, sound_index = 0;
  static uint8_t lastrandom_value = 0;
	
  srand(us_ticker_read());
  random_value = rand();
	
  if(random_value%9==lastrandom_value)
  {	  	
	  random_value ++;	  	  
  }
  lastrandom_value =random_value%9;	

  switch (random_value % 9) {
    case 0:
      add_face_action(1, 10);
      break;
    case 1:
      add_face_action(2, 10);
      break;
    case 2:
      add_face_action(3, 10);
      break;
    case 3:
      add_face_action(4, 10);
      break;
    case 4:
      add_face_action(5, 10);
      break;
    case 5:
      add_face_action(6, 10);
      break;
    case 6:
      add_face_action(7, 10);
      break;
    case 7:
      add_face_action(8, 10);
      break;
    case 8:
      add_face_action(9, 10);
      break;
    default:
      break;
  }
  //新需求轨迹模式不随机语音
  /*
  srand(us_ticker_read());
  sound_type = rand() % 12;
  srand(us_ticker_read());
  sound_index = rand() % sound_count[sound_type];

  add_sound(sound_type, sound_index, 1000);
  */
}

//充电时的表情
void OdevAction::charge_mode_action() {
//	uint8_t random_value = 0;
//	uint8_t sound_type = 0, sound_index = 0;
	static uint8_t lastrandom_value = 0;

	action_power_state = action_power_charge;
	
	
	if(lastrandom_value==0)
	add_face(face_charge_01, 1000);
	if(lastrandom_value==1)
	add_face(face_charge_02, 1000);
	if(lastrandom_value==2)
	add_face(face_charge_03, 1000);
	if(lastrandom_value==3)
	add_face(face_charge_04, 1000);
	if(lastrandom_value==4)
	add_face(face_charge_05, 1000);
	
	lastrandom_value++;
	if(lastrandom_value>=5)
		lastrandom_value = 0;
	/*
	//由于产品认证的要求不能要表情，这里用空白表情代替
	add_face(face_empty, 1000);
	*/
  /*	
  
	
  srand(us_ticker_read());
  random_value = rand();
	
  if(random_value%9==lastrandom_value)
  {	  	
	  random_value ++;	  	  
  }
  lastrandom_value =random_value%9;	

  switch (random_value % 9) {
    case 0:
      add_face_action(1, 10);
      break;
    case 1:
      add_face_action(2, 10);
      break;
    case 2:
      add_face_action(3, 10);
      break;
    case 3:
      add_face_action(4, 10);
      break;
    case 4:
      add_face_action(5, 10);
      break;
    case 5:
      add_face_action(6, 10);
      break;
    case 6:
      add_face_action(7, 10);
      break;
    case 7:
      add_face_action(8, 10);
      break;
    case 8:
      add_face_action(9, 10);
      break;
    default:
      break;
  }
  */
  //充电随机语音待定
  /*
  srand(us_ticker_read());
  sound_type = rand() % 12;
  srand(us_ticker_read());
  sound_index = rand() % sound_count[sound_type];

  add_sound(sound_type, sound_index, 1000);
  */
 }

/// 非空闲更新
void OdevAction::active() {
  idle = false;
  prev_active_tick = us_ticker_read();
}

/// 设置静默模式，不会播放声音动作
void OdevAction::set_silent_mode(uint8_t mode) {
  silent_mode = mode;
}

void OdevAction::add_set_led_matrix_callback(Callback<void(const uint8_t*)> func) {
  set_led_matrix_callback = func;
}
void OdevAction::add_set_motor_speed_callback(Callback<void(int8_t, int8_t)> func) {
  set_motor_speed_callback = func;
}
void OdevAction::add_set_sound_callback(Callback<void(uint8_t, uint8_t)> func) {
  set_sound_callback = func;
}
}
