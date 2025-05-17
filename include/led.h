#ifndef LED_H
#define LED_H

void init_leds(void);
void set_led_state(int green, int blue, int red);
void update_leds_and_buzzer(float distance_m);

#endif
