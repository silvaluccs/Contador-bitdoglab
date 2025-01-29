#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "ws2812.pio.h"


const uint pino_buzzer_a = 21;
const uint pino_buzzer_b = 10;
const uint pino_led_vermelho = 13;
const uint pino_botao_a = 5;
const uint pino_botao_b = 6;

const uint frequencia_buzzer = 10000;

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


void setup_led_vermelho();
void setup_botoes();
void setup_matriz_leds();
void setup_buzzers();

void beep(uint pino_buzzer, uint tempo);

static inline void colocar_pixel(uint32_t pixel_rgb);
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
void desenhar_numero_na_matriz_de_leds(uint8_t r, uint8_t g, uint8_t b, bool *frame_numero_atual);

void gpio_irq_handler(uint gpio, uint32_t events);

int main()
{
    stdio_init_all();

    setup_buzzers(); // inicializando os buzzers
    setup_matriz_leds(); // inicializando a matriz de leds
    setup_led_vermelho(); // inicializando o led vermelho
    setup_botoes(); // inicializando os botoes a e b

    // definindo uma interrupção para os botoes na borda de descida
    gpio_set_irq_enabled_with_callback(pino_botao_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(pino_botao_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // desenhando inicialmente o numero zero
    desenhar_numero_na_matriz_de_leds(r, g, b, buffer_numero_zero);

    while (true) {
        
        for (int i = 0; i < 4; i++) {
            gpio_put(pino_led_vermelho, true);
            sleep_ms(100);
            gpio_put(pino_led_vermelho, false);
            sleep_ms(100);           
        }

        printf("Piscando.\n");
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
* Função para inicializar os buzzers
*/
void setup_buzzers() {


    gpio_set_function(pino_buzzer_a, GPIO_FUNC_PWM);
    gpio_set_function(pino_buzzer_b, GPIO_FUNC_PWM);

    // Obter o slice do PWM associado ao pino
    uint slice_num_buzzer_a = pwm_gpio_to_slice_num(pino_buzzer_a);
    uint slice_num_buzzer_b = pwm_gpio_to_slice_num(pino_buzzer_b);

    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (frequencia_buzzer * 4096)); // Divisor de clock
    pwm_init(slice_num_buzzer_a, &config, true);
    pwm_init(slice_num_buzzer_b, &config, true);

    // Iniciar o PWM no nível baixo
    pwm_set_gpio_level(pino_buzzer_a, 0);
    pwm_set_gpio_level(pino_buzzer_b, 0);

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
* Definição de uma função para emitir um beep com duração especificada
*/ 
void beep(uint pino_buzzer, uint tempo) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pino_buzzer);

    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(pino_buzzer, 2048);

    // Temporização
    sleep_ms(100);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pino_buzzer, 0);

    // Pausa entre os beeps
    sleep_ms(100); // Pausa de 100ms
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
        
        if (gpio == pino_botao_a) {
            beep(pino_buzzer_a, 30);
        }
        else {
            beep(pino_buzzer_b, 30);
        }

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