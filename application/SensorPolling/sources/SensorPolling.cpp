#include "FreeRTOS.h"
#include "task.h"

#include "SensorPolling.h"
#include <cstdio>
#include "fdcan.h"

void SensorPolling::task() {
    float gyro_x, gyro_y, gyro_z;
    float accel_x, accel_y, accel_z;

    while (true) {
        imu.update_values();
        imu.get_gyro_values(gyro_x, gyro_y, gyro_z);
        imu.get_accel_values(accel_x, accel_y, accel_z);
        gyro_updated = true;
        accel_updated = true;
//        printf("Accel %d %d %d\n", (int)(accel_x*1000), (int)(accel_y*1000), (int)(accel_z*1000));
//        printf("Gyro %d %d %d\n", (int)(gyro_x*100), (int)(gyro_y*100), (int)(gyro_z*100));
//        uint8_t gyro_x_can = (uint8_t)(accel_x*1000);
//        fdcan.transmit(1, &gyro_x_can, 4);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

int32_t SensorPolling::init() {
    int32_t status = 0;
    if (!is_initialized) {
        BaseType_t task_status = xTaskCreate(startTaskImpl, "sensor_task", 512, this, 1, &task_handle);
        if (task_status == pdTRUE) {
            vTaskSuspend(task_handle);
            imu.init();
            is_initialized = true;
        } else {
            printf("Error сreating sensor polling task\n");
            status = 2;
        }
    } else {
        status = 1;
    }
    return status;
}

void SensorPolling::start() {
    vTaskResume(task_handle);
}

int32_t SensorPolling::get_gyro_data(float &x_axis, float &y_axis, float &z_axis) {
    int32_t status = 0;
    if (is_initialized) {
        x_axis = gyro_x;
        y_axis = gyro_y;
        z_axis = gyro_z;
        if (!gyro_updated) {
            status = 2;
        }
    } else {
        status = 1;
    }
    return status;
}

int32_t SensorPolling::get_accel_data(float &x_axis, float &y_axis, float &z_axis) {
    int32_t status = 0;
    if (is_initialized) {
        x_axis = accel_x;
        y_axis = accel_y;
        z_axis = accel_z;
        if (!accel_updated) {
            status = 2;
        }
    } else {
        status = 1;
    }
    return status;
}

int32_t SensorPolling::get_magn_data(float &x_axis, float &y_axis, float &z_axis) {
    int32_t status = 0;
    if (is_initialized) {
        x_axis = magn_x;
        y_axis = magn_y;
        z_axis = magn_z;
        if (!magn_updated) {
            status = 2;
        }
    } else {
        status = 1;
    }
    return status;
}
