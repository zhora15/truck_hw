#ifndef TEST_F411_PERIPHERY_SPI_HAL_SPI_H_
#define TEST_F411_PERIPHERY_SPI_HAL_SPI_H_

#include <cstdint>
#include "stm32g4xx_ll_spi.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_bus.h"

class HAL_SPI1 {
 private:
  HAL_SPI1() {};
  ~HAL_SPI1() {};
  HAL_SPI1(const HAL_SPI1 &obj);
  HAL_SPI1 &operator=(const HAL_SPI1 &obj);

  uint32_t transferDirection = LL_SPI_HALF_DUPLEX_TX;
  uint32_t dataWidth = LL_SPI_DATAWIDTH_8BIT;
  uint32_t time_out = 20; // 20 ms to transmit/receive
  void init();

 public:
  static HAL_SPI1 &getInstance() {
      static HAL_SPI1 _instance;
      _instance.init();
      return _instance;
  }
  void configure();
  int32_t exchange_bytes_half_duplex(uint8_t *, size_t, uint8_t *, size_t);

};
#endif //TEST_F411_PERIPHERY_SPI_HAL_SPI_H_
