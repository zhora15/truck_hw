#ifndef TRUCK_HW_APPLICATION_SENSORPOLLING_INCLUDE_SENSORPOLLING_H_
#define TRUCK_HW_APPLICATION_SENSORPOLLING_INCLUDE_SENSORPOLLING_H_

#include "FreeRTOS.h"
#include "task.h"

#include "icm20984.h"

class SensorPolling {
 private:
  ICM20984 &mems;

  TaskHandle_t task_handle;
  void task();

  static void startTaskImpl(void *_this) {
      ((SensorPolling *) _this)->task();
  }

 public:
  SensorPolling() : mems(ICM20984::getInstance()) {}
  void init();
  void start();
};

#endif //TRUCK_HW_APPLICATION_SENSORPOLLING_INCLUDE_SENSORPOLLING_H_
