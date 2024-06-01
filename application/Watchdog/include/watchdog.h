#ifndef TRUCK_HW_APPLICATION_WATCHDOG_INCLUDE_WATCHDOG_H_
#define TRUCK_HW_APPLICATION_WATCHDOG_INCLUDE_WATCHDOG_H_
#include <vector>
#include "FreeRTOS.h"
#include "task.h"

#include "stm32g4xx_hal_iwdg.h"
class Watchdog {
 private:
  bool is_initialized;
  static std::vector<std::pair<TaskHandle_t, bool>> tasks;
  Watchdog() {};
  ~Watchdog() {};
  Watchdog(const Watchdog &obj) = delete;
  Watchdog &operator=(const Watchdog &obj) = delete;
  IWDG_HandleTypeDef hiwdg;

 public:
  static Watchdog &getInstance() {
      static Watchdog _instance;
      return _instance;
  }

  void init() {
      hiwdg.Instance = IWDG;
      hiwdg.Init.Prescaler = IWDG_PRESCALER_128;
      hiwdg.Init.Reload = 2300;
      HAL_IWDG_Init(&hiwdg);
  }

  static void register_task(TaskHandle_t task) {
      tasks.emplace_back(task, false);
  };

  static void reset_watchdog(TaskHandle_t task) {
      bool reset_global_watchdog = true;
      for (auto it = tasks.begin(); it != tasks.end(); ++it) {
          if (it-> first == task) {
              it-> second = true;
          } else if (it->second == false) {
              TaskStatus_t status;
              vTaskGetInfo(task, &status, pdFALSE, eRunning);
              if (status.eCurrentState <= eSuspended) {
                  reset_global_watchdog = false;
              }
          }
      }
      if (reset_global_watchdog) {
          HAL_IWDG_Refresh(&hiwdg);
          for (auto it = tasks.begin(); it != tasks.end(); ++it) {
              it->second = false;
          }
      }
  };

};
#endif //TRUCK_HW_APPLICATION_WATCHDOG_INCLUDE_WATCHDOG_H_
