#include "pico/stdlib.h"
#include "sensor.h"

#define TRIG_PIN 14
#define ECHO_PIN 15
#define SOUND_SPEED 343

void init_ultrasonic_sensor() {
    gpio_init(TRIG_PIN);
    gpio_init(ECHO_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
    gpio_put(TRIG_PIN, 0);
}

float measure_distance() {
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    while (gpio_get(ECHO_PIN) == 0)
        tight_loop_contents();

    absolute_time_t start_time = get_absolute_time();
    while (gpio_get(ECHO_PIN) == 1)
        tight_loop_contents();
    absolute_time_t end_time = get_absolute_time();

    int64_t pulse_duration = absolute_time_diff_us(start_time, end_time);
    return (pulse_duration / 2.0) * (SOUND_SPEED / 1000000.0);
}
