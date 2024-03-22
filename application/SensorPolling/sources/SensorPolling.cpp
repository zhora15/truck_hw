#include "FreeRTOS.h"
#include "task.h"

#include "SensorPolling.h"
#include <cstdio>
#include "fdcan.h"

void SensorPolling::task() {
    float gyro_x, gyro_y, gyro_z;
    float accel_x, accel_y, accel_z;
//    vTaskDelay(20000);
    FDCAN fdcan = FDCAN();
    while (true) {
        mems.update_values();
        mems.get_gyro_values(gyro_x, gyro_y, gyro_z);
        mems.get_accel_values(accel_x, accel_y, accel_z);
        printf("Accel %d %d %d\n", (int)(accel_x*1000), (int)(accel_y*1000), (int)(accel_z*1000));
        printf("Gyro %d %d %d\n", (int)(gyro_x*100), (int)(gyro_y*100), (int)(gyro_z*100));
        uint8_t gyro_x_can = (uint8_t)(accel_x*1000);
        fdcan.transmit(1, &gyro_x_can, 4);
//        printf("Accel_float %f %f %f\n", accel_x, accel_y, accel_z);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void SensorPolling::init() {
    BaseType_t status = xTaskCreate(startTaskImpl, "Task", 4096, this, 1, &task_handle);
    if (status != pdTRUE) {
        printf("Error Creating task\n");
    }
    vTaskSuspend(task_handle);
    mems.init();
}
void SensorPolling::start() {
    vTaskResume(task_handle);
}

