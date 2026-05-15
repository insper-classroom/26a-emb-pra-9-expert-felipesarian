#include "rgb_led.h"
#include "pins.h"

#include <stdio.h>
#include <string.h>

#include "hardware/gpio.h"

void rgb_led_init(void) {
    gpio_init(LED_R_GPIO);
    gpio_init(LED_G_GPIO);
    gpio_init(LED_B_GPIO);
    gpio_set_dir(LED_R_GPIO, GPIO_OUT);
    gpio_set_dir(LED_G_GPIO, GPIO_OUT);
    gpio_set_dir(LED_B_GPIO, GPIO_OUT);
    rgb_led_set(0, 0, 0);
}

void rgb_led_set(int r, int g, int b) {
    gpio_put(LED_R_GPIO, r ? 1 : 0);
    gpio_put(LED_G_GPIO, g ? 1 : 0);
    gpio_put(LED_B_GPIO, b ? 1 : 0);
}

void rgb_led_show_class(const char *label) {
    if (label == NULL) {
        rgb_led_set(0, 0, 0);
        printf("none\n");
        return;
    }
    if (strcmp(label, "wave") == 0) {
        rgb_led_set(1, 0, 0);
        printf("wave\n");
    } else if (strcmp(label, "updown") == 0) {
        rgb_led_set(0, 1, 0);
        printf("updown\n");
    } else if (strcmp(label, "idle") == 0) {
        rgb_led_set(0, 0, 1);
        printf("idle\n");
    } else {
        rgb_led_set(0, 0, 0);
        printf("none\n");
    }
}
