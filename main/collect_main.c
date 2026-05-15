#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"

#include "mpu6050.h"

#define SAMPLE_PERIOD_MS 10  // 100 Hz

static void mpu_task(void *pv) {
    (void)pv;
    mpu6050_init();

    int16_t accel[3], gyro[3], temp;
    TickType_t next = xTaskGetTickCount();

    for (;;) {
        mpu6050_read_raw(accel, gyro, &temp);
        printf("%d,%d,%d\n", accel[0], accel[1], accel[2]);
        vTaskDelayUntil(&next, pdMS_TO_TICKS(SAMPLE_PERIOD_MS));
    }
}

int main(void) {
    stdio_init_all();

    xTaskCreate(mpu_task, "mpu", 4096, NULL, 1, NULL);
    vTaskStartScheduler();

    for (;;) {
    }
}
