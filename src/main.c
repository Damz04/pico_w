#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/err.h"
#include "lcd.h"
#include "sensor.h"
#include "led.h"
#include "buzzer.h"
#include "wifi.h"
#include "mqtt.h"

// Define pins for components
#define TRIG_PIN 14
#define ECHO_PIN 15
#define SOUND_SPEED 343 // Speed of sound in m/s
#define LWIP_MQTT 1
#define GREEN_LED 2
#define BLUE_LED 3
#define RED_LED 4
#define BUZZER_PIN 5

int main() {
    stdio_init_all();                                // Initialize standard I/O (for printf, etc.)
    sleep_ms(3000);                                  // Wait for peripherals to initialize


    printf("Entering main program...\n");

    init_ultrasonic_sensor();
    init_leds();
    init_buzzer();

    sleep_ms(5000);

    // Attempt to connect to Wi-Fi. Replace with your own wifi credentials:
    if (!connect_to_wifi("wifi-ssid", "wifi-password")) {  
        return 1;
    }

    mqtt_client_t *client;
    setup_mqtt_client(&client);

    extern volatile bool alarm_synced;
    
    // Wait for MQTT connection to be established
    while (!mqtt_connected) {
        printf("Waiting for MQTT connection...\n");
        sleep_ms(500);
    }
    
    // Wait for alarm state to be synced with MQTT broker
    while (!alarm_synced) {
        printf("Waiting for alarm state sync...\n");
        sleep_ms(500);
    }

    while (true) {
        printf("Connected!\n");
        // Print IP address
        uint8_t *ip = (uint8_t*)&cyw43_state.netif[0].ip_addr.addr;
        printf("IP: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);

        // Measure distance
        for (int i = 0; i < 5; i++)
        {
            float distance = measure_distance();
            if (distance <= 0 || distance > 500.0f) {
                // Ignore garbage values or sensor misreads
                continue;
            } else {
                update_leds_and_buzzer(distance);
                publish_distance(client, distance);
            }

            sleep_ms(2000); // Delay between readings
        }
    }

    return 0;
}