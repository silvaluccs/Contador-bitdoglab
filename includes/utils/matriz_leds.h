#ifndef MATRIZ_LEDS_H
#define MATRIZ_LEDS_H

#include "pico/stdlib.h"

static inline void colocar_pixel(uint32_t pixel_rgb);
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
void desenhar_numero_na_matriz_de_leds(uint8_t r, uint8_t g, uint8_t b, uint numero_atual);
static void desenhar(uint8_t r, uint8_t g, uint8_t b, bool *frame_numero_atual);

#endif