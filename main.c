#include <stdio.h> // biblioteca para usar printf
#include "pico/stdlib.h" // biblioteca para usar stdio_init_all
#include "hardware/pio.h"  // biblioteca para usar pio
#include "hardware/timer.h" // biblioteca para usar timer
#include "hardware/pwm.h" // biblioteca para usar pwm
#include "ws2812.pio.h" // biblioteca para usar ws2812
#include "setup.h" // biblioteca para usar as funções de setup  
#include "led.h" // biblioteca para usar as funções do led
#include "matriz_leds.h" // biblioteca para usar as funções da matriz de leds


const uint pino_led_vermelho = 13; // pino do led vermelho
const uint pino_botao_a = 5; // pino do botao A
const uint pino_botao_b = 6; // pino do botao B
const uint pino_matriz_leds = 7; // pino da matriz de leds


static volatile uint32_t ultimo_tempo = 0; // variavel para debouce
static volatile uint numero_atual = 0;  // variavel para armazenar o numero atual


const uint r = 10, b = 0, g = 0; // valores de r, g e b para a matriz de leds


void gpio_irq_handler(uint gpio, uint32_t events); // prototipo da função para tratar a interrupção dos botoes

int main()
{
    stdio_init_all(); // inicializando a comunicação serial

    setup_led_vermelho(pino_led_vermelho); // inicializando o led vermelho
    setup_botoes(pino_botao_a, pino_botao_b); // inicializando os botoes
    setup_matriz_leds(pino_matriz_leds);  // inicializando a matriz de leds


    // definindo uma interrupção para os botoes na borda de descida
    gpio_set_irq_enabled_with_callback(pino_botao_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // botao A
    gpio_set_irq_enabled_with_callback(pino_botao_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // botao B
 
    // desenhando inicialmente o numero zero
    desenhar_numero_na_matriz_de_leds(r, g, b, numero_atual); // desenhando o numero zero na matriz de leds

    while (true) {
       piscar_led(pino_led_vermelho); // piscando o led vermelho
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
        ++numero_atual; // incrementando o numero
        printf("Botão A pressionado. ");
        printf("Número atual: %d\n", numero_atual);
    }

    else if (gpio == pino_botao_b && numero_atual > 0) { // verificando se o numero esta na faixa adequada
        --numero_atual; // decrementando o numero
        printf("Botão B pressionado. ");
        printf("Número atual: %d\n", numero_atual);
    }
    else { // caso o numero nao esteja na faixa adequada
        return;
    }

    desenhar_numero_na_matriz_de_leds(r, g, b, numero_atual); // desenhando o numero na matriz de leds

}