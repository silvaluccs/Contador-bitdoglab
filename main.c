#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"


const uint pino_led_vermelho = 13;
const uint pino_botao_a = 5;
const uint pino_botao_b = 6;

const uint numero_pixels = 25;
const uint pino_matriz_leds = 7;
const bool IS_RGBW = false;


void setup_led_vermelho();
void setup_botoes();
void setup_matriz_leds();

static inline void colocar_pixel(uint32_t pixel_rgb);
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
void desenhar_numero_na_matriz_de_leds(uint8_t r, uint8_t g, uint8_t b, uint numero, bool *frame_numero_atual);

int main()
{
    stdio_init_all();

    setup_led_vermelho(); // inicializando o led vermelho
    setup_botoes(); // inicializando os botoes a e b

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

/*
* Função para inicializar a matriz de leds da placa
*/
void setup_matriz_leds() {

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, pino_matriz_leds, 800000, IS_RGBW);
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
void desenhar_numero_na_matriz_de_leds(uint8_t r, uint8_t g, uint8_t b, uint numero, bool *frame_numero_atual)
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
