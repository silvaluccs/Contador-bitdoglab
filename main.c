#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

const uint pino_led_vermelho = 13;
const uint pino_botao_a = 5;
const uint pino_botao_b = 6;

void setup_led_vermelho();
void setup_botoes();

int main()
{
    stdio_init_all();

    // inicializando o led vermelho
    setup_led_vermelho();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}

/*
* Função para inicializar o led vermelho
*/
void setup_led_vermelho() {

    gpio_init(pino_led_vermelho); // inicializando o pino
    gpio_set_dir(pino_led_vermelho, GPIO_OUT); // definindo como saida
    gpio_put(pino_led_vermelho, false); // deixando desligado inicialmente

}

/*
* Função para inicializar os botões a e b
*/
void setup_botoes() {

    gpio_init(pino_botao_a); // inicializando o pino
    gpio_set_dir(pino_botao_a, GPIO_IN); // defininfo como entrada
    gpio_pull_up(pino_botao_a); // ativando resistores internos

    gpio_init(pino_botao_b); // inicializando o pino
    gpio_set_dir(pino_botao_b, GPIO_IN); // definindo como entrada
    gpio_pull_up(pino_botao_b); // ativando resistores internos

}