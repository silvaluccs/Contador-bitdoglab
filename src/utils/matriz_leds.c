#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "ws2812.pio.h"
#include "matriz_leds.h"


const uint numero_pixels = 25;

static bool buffer_numero_zero[25] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
};

static bool buffer_numero_um[25] = {
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
};

static bool buffer_numero_dois[25] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
};

static bool buffer_numero_tres[25] = {
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
};

static bool buffer_numero_quatro[25] = {
    0, 1, 0, 0, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
};


static bool buffer_numero_cinco[25] = {
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
};

static bool buffer_numero_seis[25] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
};


static bool buffer_numero_sete[25] = {
    0, 1, 0, 0, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 0, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
};


static bool buffer_numero_oito[25] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
};


static bool buffer_numero_nove[25] = {
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
};



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
void desenhar_numero_na_matriz_de_leds(uint8_t r, uint8_t g, uint8_t b, uint numero_atual) {

    // desenhando o numero atual na matriz de leds
    switch(numero_atual) {

        case 0:
            desenhar(r, g, b, buffer_numero_zero);
            break;
        case 1:
            desenhar(r, g, b, buffer_numero_um);
            break;
        case 2:
            desenhar(r, g, b, buffer_numero_dois);
            break;
        case 3:
            desenhar(r, g, b, buffer_numero_tres);
            break;
        case 4:
            desenhar(r, g, b, buffer_numero_quatro);
            break;
        case 5:
            desenhar(r, g, b, buffer_numero_cinco);
            break;
        case 6:
            desenhar(r, g, b, buffer_numero_seis);
            break;
        case 7:
            desenhar(r, g, b, buffer_numero_sete);
            break;
        case 8:
            desenhar(r, g, b, buffer_numero_oito);
            break;
        case 9:
            desenhar(r, g, b, buffer_numero_nove);
            break;
        default:
            break;
    }

}

/*
* Função para enviar um desenho para matriz de leds
*/
static void desenhar(uint8_t r, uint8_t g, uint8_t b, bool *frame_numero_atual)
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
