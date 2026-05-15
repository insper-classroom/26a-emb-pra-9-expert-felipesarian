#ifndef RGB_LED_H
#define RGB_LED_H

#ifdef __cplusplus
extern "C" {
#endif

void rgb_led_init(void);
void rgb_led_set(int r, int g, int b);

// Mapeia rótulo da classe vencedora para uma cor, liga o LED e imprime
// "red"/"green"/"blue" na serial. Para rótulos desconhecidos imprime "none".
void rgb_led_show_class(const char *label);

#ifdef __cplusplus
}
#endif

#endif
