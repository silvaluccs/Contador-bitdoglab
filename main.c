#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "ws2812.pio.h"
#include "setup.h"
#include "led.h"

const uint pino_led_vermelho = 13;
const uint pino_botao_a = 5;
const uint pino_botao_b = 6;

#define numero_pixels 25
const uint pino_matriz_leds = 7;
const bool IS_RGBW = false;

static volatile uint32_t ultimo_tempo = 0;
static volatile uint numero_atual = 0;


const uint r = 10, b = 0, g = 0;

bool buffer_numero_zero[numero_pixels] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
};

bool buffer_numero_um[numero_pixels] = {
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
};

bool buffer_numero_dois[numero_pixels] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
};

bool buffer_numero_tres[numero_pixels] = {
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
};

bool buffer_numero_quatro[numero_pixels] = {
    0, 1, 0, 0, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
};


bool buffer_numero_cinco[numero_pixels] = {
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
};

bool buffer_numero_seis[numero_pixels] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
};

bool buffer_numero_sete[numero_pixels] = {
    0, 1, 0, 0, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 0, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
};


bool buffer_numero_oito[numero_pixels] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
};


bool buffer_numero_nove[numero_pixels] = {
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
};


static inline void colocar_pixel(uint32_t pixel_rgb);
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
void desenhar_numero_na_matriz_de_leds(uint8_t r, uint8_t g, uint8_t b, bool *frame_numero_atual);

void gpio_irq_handler(uint gpio, uint32_t events);

int main()
{
    stdio_init_all();

    setup_led_vermelho(pino_led_vermelho);
    setup_botoes(pino_botao_a, pino_botao_b);
    setup_matriz_leds(pino_matriz_leds, IS_RGBW);


    // definindo uma interrupção para os botoes na borda de descida
    gpio_set_irq_enabled_with_callback(pino_botao_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(pino_botao_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // desenhando inicialmente o numero zero
    desenhar_numero_na_matriz_de_leds(r, g, b, buffer_numero_zero);

    while (true) {
       piscar_led(pino_led_vermelho);
       sleep_ms(1000);
    }
}



/*
* Função para enviar o pixel para a matriz de leds
*/
static inline void colocar_pixel(uint32_t pixel_rgb) {
    pio_sm_put_blocking(pio0, 0, pixel_rgb << 8u);
}


/*
* Função para converter os valores rgb
*/
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}


/*
* Função para desenhar um número especifico na matriz de leds
*/
void desenhar_numero_na_matriz_de_leds(uint8_t r, uint8_t g, uint8_t b, bool *frame_numero_atual)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t cor = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < numero_pixels; i++)
    {
        if (frame_numero_atual[i])
        {
            colocar_pixel(cor); // Liga o LED com um no buffer
        }
        else
        {
            colocar_pixel(0);  // Desliga os LEDs com zero no buffer
        }
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

    // desenhando o numero atual na matriz de leds
    switch(numero_atual) {

        case 0:
            desenhar_numero_na_matriz_de_leds(r, g, b, buffer_numero_zero);
            break;
        case 1:
            desenhar_numero_na_matriz_de_leds(r, g, b, buffer_numero_um);
            break;
        case 2:
            desenhar_numero_na_matriz_de_leds(r, g, b, buffer_numero_dois);
            break;
        case 3:
            desenhar_numero_na_matriz_de_leds(r, g, b, buffer_numero_tres);
            break;
        case 4:
            desenhar_numero_na_matriz_de_leds(r, g, b, buffer_numero_quatro);
            break;
        case 5:
            desenhar_numero_na_matriz_de_leds(r, g, b, buffer_numero_cinco);
            break;
        case 6:
            desenhar_numero_na_matriz_de_leds(r, g, b, buffer_numero_seis);
            break;
        case 7:
            desenhar_numero_na_matriz_de_leds(r, g, b, buffer_numero_sete);
            break;
        case 8:
            desenhar_numero_na_matriz_de_leds(r, g, b, buffer_numero_oito);
            break;
        case 9:
            desenhar_numero_na_matriz_de_leds(r, g, b, buffer_numero_nove);
            break;
        default:
            break;
    }

}