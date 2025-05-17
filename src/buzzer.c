#include "pico/stdlib.h"
#include "buzzer.h"

// Define the GPIO pin connected to the buzzer
#define BUZZER_PIN 5

// Initialize the buzzer pin as an output
void init_buzzer() {
    gpio_init(BUZZER_PIN);                        // Initialize the GPIO pin
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);           // Set pin as output
}

// Activate the buzzer for a short beep (200 ms)
void buzz_short() {
    gpio_put(BUZZER_PIN, 1);                     // Turn buzzer ON
    sleep_ms(200);                               // Wait 200 milliseconds 
    gpio_put(BUZZER_PIN, 0);                     // Turn buzzer OFF
}

// Turn the buzzer ON indefinitely (until manually turned off)
void buzz_long() {
    gpio_put(BUZZER_PIN, 1);                    // Keep buzzer ON
}

// Turn the buzzer OFF
void buzz_off() {
    gpio_put(BUZZER_PIN, 0);                    // Set pin LOW to stop buzzing
}
