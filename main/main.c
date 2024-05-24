#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const int BTN_PIN_B = 2;
const int BTN_PIN_G = 13;
const int BUZZER_PIN = 15;
volatile char PRESSED = ' ';

void set_pins();
void btn_callback(uint gpio, uint32_t events);
void play_buzzer(int frequency, int duration);

int main() {
    stdio_init_all();
    set_pins();
    
    // Configurar interrupções para os botões
    gpio_set_irq_enabled_with_callback(BTN_PIN_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    while (true) {
        if (PRESSED == 'b') {
            play_buzzer(1000, 100); // Toca um som de 1kHz por 100ms
        } else if (PRESSED == 'g') {
            play_buzzer(6000, 300); // Toca um som de 6kHz por 300ms
        }
        sleep_ms(10); // Pequena pausa para evitar uso excessivo da CPU
    }
}

void set_pins() {
    gpio_init(BTN_PIN_B);
    gpio_set_dir(BTN_PIN_B, GPIO_IN);
    gpio_pull_up(BTN_PIN_B);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
}

void btn_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) { // Se houver borda de subida
        if (gpio == BTN_PIN_B) {
            PRESSED = 'b';
        } 
        if (gpio == BTN_PIN_G) {
            PRESSED = 'g';
        }
    }
}

void play_buzzer(int frequency, int duration) {
    int sleep_time = 1000000 / (frequency * 2); // Tempo de meio período em microsegundos
    int steps = (duration * frequency) / 1000;  // Número de ciclos necessários

    for (int i = 0; i < steps; i++) {
        gpio_put(BUZZER_PIN, 1);
        sleep_us(sleep_time);
        gpio_put(BUZZER_PIN, 0);
        sleep_us(sleep_time);
    }
    PRESSED = ' '; // Reinicia a variável PRESSED após tocar o som
}
