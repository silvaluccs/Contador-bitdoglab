#ifndef SETUP_H
#define SETUP_H

#include "pico/stdlib.h"

void setup_led_vermelho(uint pino_led_vermelho);
void setup_botoes(uint pino_botao_a, uint pino_botao_b);
void setup_matriz_leds(uint pino_matriz_leds);

#endif