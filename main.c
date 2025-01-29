#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

const uint pino_led_vermelho = 13;

void setup_led_vermelho();

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
    gpio_set_dir(pino_led_vermelho, GPIO.OUT); // definindo como saida
    gpio_put(pino_led_vermelho, false); // deixando desligado inicialmente

}