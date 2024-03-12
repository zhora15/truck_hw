#include "MotorControl.h"

#include "performance_counter.h"
#include "BLDCMotor.h"
#include "hal_gpio.h"

void MotorControl::task() {
    uint32_t start_tick = HAL_GetTick();
    const float speed = 2.0f * 3.1415f * 50;

    while (true) {
//        if (((HAL_GetTick() - start_tick) / 100.0 > speed) && (speed < 50)) {
//            speed += 0.1;
//        }
//        printf("Minimum ever free heap size - %d byte(s) - %d\n",
//                xPortGetMinimumEverFreeHeapSize(), HAL_GetTick());
        hal_gpio_set_pin(GPIO_PORT_B, GPIO_PIN_1);
        motor.loopFOC();
        motor.move(speed);
//        printf("%d\n", (int32_t) (mag_enc.getAngle()/6.28*360));
        hal_gpio_reset_pin(GPIO_PORT_B, GPIO_PIN_1);
        vTaskDelay(2);
    }
}

void MotorControl::init() {
    driver.pwm_frequency = 25000;
    motor.voltage_limit = 2;
    driver.voltage_limit = 2;
    driver.voltage_power_supply = 15;
    motor.controller = MotionControlType::velocity;

    driver.init();
    motor.linkDriver(&driver);
    motor.linkSensor(&mag_enc);
    mag_enc.needsSearch();
    BaseType_t status = xTaskCreate(startTaskImpl, "Task", 4096, this, 1, &task_handle);
    if (status != pdTRUE) {
        printf("Error Creating task\n");
    }
    vTaskSuspend(task_handle);
}

void MotorControl::calibrate() {
    motor.init();
    motor.initFOC();
    hal_gpio_init(GPIO_PORT_B, GPIO_PIN_1);
    vTaskResume(task_handle);
}