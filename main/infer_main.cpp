
#include <cstdio>
#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"

#include "mpu6050.h"
#include "rgb_led.h"

#include "edge-impulse-sdk/classifier/ei_model_types.h"
#include "edge-impulse-sdk/dsp/numpy.hpp"
#include "model-parameters/model_metadata.h"

using namespace ei;

extern "C" EI_IMPULSE_ERROR
run_classifier(ei::signal_t *signal, ei_impulse_result_t *result, bool debug);

extern "C" void run_classifier_init(void);

static const bool debug_nn = false;

static const float CLASSIFY_THRESHOLD = 0.5f;

static void gesture_task(void *pv) {
    (void)pv;

    printf("[boot] gesture_task iniciada\n");
    mpu6050_init();
    printf("[boot] mpu6050_init ok\n");
    rgb_led_init();
    printf("[boot] janela=%lu samples a 10 ms (=%d Hz)\n",
           (unsigned long)EI_CLASSIFIER_RAW_SAMPLE_COUNT,
           EI_CLASSIFIER_FREQUENCY);

    int16_t accelerometer[3], gyro[3], temp;

    while (true) {
        float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };

        for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 3) {
            mpu6050_read_raw(accelerometer, gyro, &temp);
            buffer[ix + 0] = (float)accelerometer[0];
            buffer[ix + 1] = (float)accelerometer[1];
            buffer[ix + 2] = (float)accelerometer[2];
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        ei::signal_t signal;
        int err = numpy::signal_from_buffer(
            buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
        if (err != 0) {
            printf("ERR signal_from_buffer: %d\n", err);
            continue;
        }

        run_classifier_init();

        ei_impulse_result_t result = {};
        EI_IMPULSE_ERROR rerr = run_classifier(&signal, &result, debug_nn);
        if (rerr != EI_IMPULSE_OK) {
            printf("ERR run_classifier: %d\n", (int)rerr);
            continue;
        }

        printf("dsp=%dms cls=%dms | ",
               (int)result.timing.dsp,
               (int)result.timing.classification);
        int argmax = 0;
        float best = result.classification[0].value;
        for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
            float v = result.classification[i].value;
            printf("%s=%.2f ", result.classification[i].label, v);
            if (v > best) {
                best = v;
                argmax = i;
            }
        }
        printf("=> ");

        if (best >= CLASSIFY_THRESHOLD) {
            rgb_led_show_class(result.classification[argmax].label);
        } else {
            rgb_led_show_class(nullptr);
        }
    }
}

int main(void) {
    stdio_init_all();

    xTaskCreate(gesture_task, "gesture", 8192, nullptr, 1, nullptr);
    vTaskStartScheduler();

    for (;;) {}
}
