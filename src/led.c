#include "pico/stdlib.h"
#include "led.h"
#include "buzzer.h"
#include "mqtt.h"

// Define GPIO pins for each LED
#define GREEN_LED 2
#define BLUE_LED 3
#define RED_LED 4

// Initialize LED pins as outputs
void init_leds() {
    gpio_init(GREEN_LED);
    gpio_set_dir(GREEN_LED, GPIO_OUT);
    gpio_init(BLUE_LED);
    gpio_set_dir(BLUE_LED, GPIO_OUT);
    gpio_init(RED_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
}

// Set the state of each LED individually (1 = ON, 0 = OFF)
void set_led_state(int green, int blue, int red) {
    gpio_put(GREEN_LED, green);
    gpio_put(BLUE_LED, blue);
    gpio_put(RED_LED, red);
}

// Update LEDs and buzzer based on distance measured
void update_leds_and_buzzer(float distance_m) {
     if (!alarm_enabled) {
        set_led_state(0, 0, 0);
        buzz_off();
        return;
    }

    if (distance_m > 1.0f) {
        set_led_state(1, 0, 0);
        buzz_off();
    } else if (distance_m > 0.2f) {
        set_led_state(0, 1, 0);
        buzz_short();
    } else {
        set_led_state(0, 0, 1);
        buzz_long();
    }
}
