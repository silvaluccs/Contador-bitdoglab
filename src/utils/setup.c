#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "ws2812.pio.h"
#include "setup.h"

#define IS_RGBW false

/*
* Função para inicializar o led vermelho
*/
void setup_led_vermelho(uint pino_led_vermelho) {

    gpio_init(pino_led_vermelho); // inicializando o pino
    gpio_set_dir(pino_led_vermelho, GPIO_OUT); // definindo como saida
    gpio_put(pino_led_vermelho, false); // deixando desligado inicialmente

}


/*
* Função para inicializar os botões a e b
*/
void setup_botoes(uint pino_botao_a, uint pino_botao_b) {

    gpio_init(pino_botao_a); // inicializando o pino
    gpio_set_dir(pino_botao_a, GPIO_IN); // defininfo como entrada
    gpio_pull_up(pino_botao_a); // ativando resistores internos

    gpio_init(pino_botao_b); // inicializando o pino
    gpio_set_dir(pino_botao_b, GPIO_IN); // definindo como entrada
    gpio_pull_up(pino_botao_b); // ativando resistores internos

}



/*
* Função para inicializar a matriz de leds da placa
*/
void setup_matriz_leds(uint pino_matriz_leds) {

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, pino_matriz_leds, 800000, IS_RGBW);
}