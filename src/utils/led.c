#include "pico/stdlib.h"
#include <stdio.h>
#include "led.h"

/*
* Função para piscar o LED
*/
void piscar_led(uint pino_led) {
    for (int i = 0; i < 4; i++) {
        gpio_put(pino_led, true);
        sleep_ms(100);
        gpio_put(pino_led, false);
        sleep_ms(100);
    }
    printf("Piscando o LED\n");
}