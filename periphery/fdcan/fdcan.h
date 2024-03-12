#ifndef TRUCK_HW_PERIPHERY_FDCAN_FDCAN_H_
#define TRUCK_HW_PERIPHERY_FDCAN_FDCAN_H_
#include "stm32g4xx_hal_fdcan.h"

class FDCAN {
 private:
  FDCAN_HandleTypeDef hfdcan1;
 public:
  void transmit();
  FDCAN();
};
#endif //TRUCK_HW_PERIPHERY_FDCAN_FDCAN_H_
