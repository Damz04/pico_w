#include "pico/stdlib.h"
#include "buzzer.h"

#define BUZZER_PIN 5

void init_buzzer() {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
}

void buzz_short() {
    gpio_put(BUZZER_PIN, 1);
    sleep_ms(200);
    gpio_put(BUZZER_PIN, 0);
}

void buzz_long() {
    gpio_put(BUZZER_PIN, 1);
}

void buzz_off() {
    gpio_put(BUZZER_PIN, 0);
}
