#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "ws2812.pio.h"
#include "setup.h"
#include "led.h"
#include "matriz_leds.h"

const uint pino_led_vermelho = 13;
const uint pino_botao_a = 5;
const uint pino_botao_b = 6;
const uint pino_matriz_leds = 7;

static volatile uint32_t ultimo_tempo = 0;
static volatile uint numero_atual = 0;

const uint r = 10, b = 0, g = 0;

void gpio_irq_handler(uint gpio, uint32_t events);

int main()
{
    stdio_init_all();

    setup_led_vermelho(pino_led_vermelho);
    setup_botoes(pino_botao_a, pino_botao_b);
    setup_matriz_leds(pino_matriz_leds);


    // definindo uma interrupção para os botoes na borda de descida
    gpio_set_irq_enabled_with_callback(pino_botao_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(pino_botao_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // desenhando inicialmente o numero zero
    desenhar_numero_na_matriz_de_leds(r, g, b, numero_atual);

    while (true) {
       piscar_led(pino_led_vermelho);
       sleep_ms(1000);
    }
}


/*
* Função para tratar a interrupção dos botoes
*/
void gpio_irq_handler(uint gpio, uint32_t events) {

    // fazendo o debouce
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());

    bool debouce = tempo_atual - ultimo_tempo > 200000;

    if (!debouce) { // caso nao tenha passado o intervalo ideal entre pressionar os botoes
        return;
    }

    ultimo_tempo = tempo_atual; // atualizando o tempo

    if (gpio == pino_botao_a && numero_atual < 9) { // verificando se o numero esta na faixa adequada
        ++numero_atual;
        printf("Botão A pressionado. ");
        printf("Número atual: %d\n", numero_atual);
    }

    else if (gpio == pino_botao_b && numero_atual > 0) { // verificando se o numero esta na faixa adequada
        --numero_atual;
        printf("Botão B pressionado. ");
        printf("Número atual: %d\n", numero_atual);
    }
    else {
        return;
    }

    desenhar_numero_na_matriz_de_leds(r, g, b, numero_atual);

}