#include <cstdio>
#include "Servo.h"

int32_t Servo::init() {
    switch (type_) {
        case SERVO_LEFT:
            max_angle = 120; // 90 + 20  TODO
            min_angle = 60; // 90 - 20
//            max_angle = 180; // 90 + 20
//            min_angle = 0; // 90 - 30
            break;
        case SERVO_RIGHT:
            max_angle = 120; // 90 + 20
            min_angle = 60; // 90 - 20 Todo
//            max_angle = 180; // 90 + 20
//            min_angle = 0; // 90 - 30
            break;
    }
    is_initialized = true;
    // TODO Self-test??
    return 0;
}

int32_t Servo::set_angle(float angle) {
    int32_t status = 0;
    if (is_initialized) {
        if (pwm.initialized()) {
            if ((angle <= max_angle) && (angle >= min_angle)) {
                uint32_t us = 500 + (uint32_t) ((2500.0 - 500.0) / 180.0 * angle);
                pwm.set_us_impulse(us);
            } else {
                status = 3;
            }
        } else {
            status = 2;
        }
    } else {
        status = 1;
    }
    return status;
}

float Servo::get_angle() {
    // TODO return measured from potentiometr angle
    return servo_angle;
}

int32_t Servo::stop() {
    int32_t status = 0;
    if (is_initialized) {
        status = pwm.stop();
    } else {
        status = 1;
    }

    return status;
}
