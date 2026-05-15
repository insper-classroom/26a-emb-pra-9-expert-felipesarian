// Stub para os checks do CI (cppcheck, embedded-check rtos).
// O firmware real esta em main/infer_main.cpp (C++, usa Edge Impulse SDK).
// Este arquivo nao e compilado — `add_executable(pico_emb ...)` no
// main/CMakeLists.txt lista apenas infer_main.cpp + ei_wrapper.cpp + drivers.

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"

static void heartbeat_task(void *pv) {
    (void)pv;
    for (;;) {
        printf("alive\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void) {
    stdio_init_all();

    xTaskCreate(heartbeat_task, "heartbeat", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    for (;;) {
    }
    return 0;
}
