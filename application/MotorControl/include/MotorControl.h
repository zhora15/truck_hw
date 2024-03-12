#ifndef TRUCK_HW_APPLICATION_MOTOR_CONTROL_INCLUDE_MOTOR_CONTROL_H_
#define TRUCK_HW_APPLICATION_MOTOR_CONTROL_INCLUDE_MOTOR_CONTROL_H_

#include <memory>
#include "MagneticEncoderTLE5012.cpp"
#include "Driver6PWM.h"
#include "BLDCMotor.h"
#include "FreeRTOS.h"
#include "task.h"

class MotorControl {
 private:
  BLDCMotor motor;
  Driver6PWM driver;
  MagneticEncoderTLE5012 mag_enc;
  TaskHandle_t task_handle;
  void task();

  static void startTaskImpl(void* _this){
      ((MotorControl*)_this)->task();
  }

 public:
  MotorControl() : mag_enc(MagneticEncoderTLE5012(TLE5012::getInstance())), motor(BLDCMotor(7)), driver(Driver6PWM()) {}
  void init();
  void calibrate();

};
#endif //TRUCK_HW_APPLICATION_MOTOR_CONTROL_INCLUDE_MOTOR_CONTROL_H_
