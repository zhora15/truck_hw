#ifndef TRUCK_HW_PERIPHERY_FDCAN_FDCAN_H_
#define TRUCK_HW_PERIPHERY_FDCAN_FDCAN_H_
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_fdcan.h"

extern FDCAN_HandleTypeDef fdcan1;

class FDCAN {
 private:
  FDCAN_HandleTypeDef fdcan_handle;
 public:
  int32_t get_message(uint32_t &message_id, uint8_t *rx_data, uint32_t &rx_data_size);
  void set_rx_complete_callback(void(*callback)());
  void transmit(uint32_t message_id, uint8_t *p_data, uint32_t size);
  FDCAN();
};
#endif //TRUCK_HW_PERIPHERY_FDCAN_FDCAN_H_
