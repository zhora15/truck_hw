//
// Created by Георгий Сенин on 05.04.2024.
//

#ifndef TRUCK_HW_CHIPSET_SERVO_INCLUDE_SERVO_H_
#define TRUCK_HW_CHIPSET_SERVO_INCLUDE_SERVO_H_

#include <cstdint>
#include "pwm_servo.h"

typedef enum {
  SERVO_LEFT,
  SERVO_RIGHT
} servo_type;

class Servo {
 private:
  float servo_angle = 0.0f;
  servo_type type_;
  PWMServo &pwm;
  bool is_initialized = false;

  float max_angle = 180;
  float min_angle = 0;
  int32_t init();
  static PWMServo &get_pwm_instance(servo_type servo_type_) {
      switch (servo_type_) {
          case SERVO_LEFT: return PWMServo::get_instance(PWM_SERVO_1);
          case SERVO_RIGHT: return PWMServo::get_instance(PWM_SERVO_2);
      }
  }
  Servo(servo_type init_type) : pwm(get_pwm_instance(init_type)) {
      type_ = init_type;
      init();
  };

  ~Servo() {};
  Servo(const Servo &obj) = delete;
  Servo &operator=(const Servo &obj) = delete;

 public:
  static Servo &get_instance(servo_type get_type) {
      switch (get_type) {
          case SERVO_LEFT:
              static Servo Servo_left(get_type);
              Servo_left.type_ = SERVO_LEFT;
              return Servo_left;
          case SERVO_RIGHT:
              static Servo Servo_right(get_type);
              Servo_right.type_ = SERVO_RIGHT;
              return Servo_right;
          default: break;
      }
  }

  int32_t set_angle(float angle);
  float get_angle();
  int32_t stop();
};

#endif //TRUCK_HW_CHIPSET_SERVO_INCLUDE_SERVO_H_
