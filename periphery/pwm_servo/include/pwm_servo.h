//
// Created by Георгий Сенин on 04.04.2024.
//

#ifndef TRUCK_HW_PERIPHERY_PWM_SERVO_INCLUDE_PWM_SERVO_H_
#define TRUCK_HW_PERIPHERY_PWM_SERVO_INCLUDE_PWM_SERVO_H_

#include "stm32g4xx_ll_tim.h"

typedef enum {
  PWM_SERVO_1,
  PWM_SERVO_2
} pwm_servo_type;

class PWMServo {
 private:
  pwm_servo_type type_;
  bool is_initialized = false;
  static bool is_common_tim_initialized;
  const uint32_t tim_preload = 144;
  const uint32_t tim_autoreload = 20000;
  int32_t init();

  PWMServo(pwm_servo_type init_type) {
      type_ = init_type;
      init();
  };
  ~PWMServo() {};
  PWMServo(const PWMServo &obj) = delete;
  PWMServo &operator=(const PWMServo &obj) = delete;

 public:
  static PWMServo &get_instance(pwm_servo_type get_type){
      static PWMServo Servo1(PWM_SERVO_1);
      static PWMServo Servo2(PWM_SERVO_2);
      Servo1.type_ = PWM_SERVO_1;
      Servo2.type_ = PWM_SERVO_2;
      switch (get_type) {
          case PWM_SERVO_1: return Servo1;
          case PWM_SERVO_2: return Servo2;
          default: break;
      }
  }

  int32_t set_us_impulse(uint32_t us);
  int32_t stop();
  bool initialized(){
      return is_initialized;
  }
};
#endif //TRUCK_HW_PERIPHERY_PWM_SERVO_INCLUDE_PWM_SERVO_H_
