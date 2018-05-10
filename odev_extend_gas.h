#ifndef __ODEV_EXTEND_GAS__
#define __ODEV_EXTEND_GAS__

#include "mbed.h"

namespace Odev {
class OdevExtendGas {
 public:
  OdevExtendGas(PinName, PinName);
  uint8_t get_gas_state();
  uint16_t get_gas_value();

 private:
  DigitalIn p1;
  AnalogIn p2;
};
}
#endif
