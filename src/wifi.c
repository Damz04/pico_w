#include <stdio.h>
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "wifi.h"

// Configuration constants
#define WIFI_MAX_RETRIES 2                                    // Maximum number of connection attempts
#define WIFI_RETRY_DELAY_MS 5000                              // Delay between retries (in milliseconds)

// Connect to a Wi-Fi network using the provided SSID and password
bool connect_to_wifi(const char *ssid, const char *pass) {
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return false;
    }

    cyw43_arch_enable_sta_mode();
    sleep_ms(1000); // Give hardware time to stabilize

    for (int attempt = 1; attempt <= WIFI_MAX_RETRIES; attempt++) {
        printf("Attempt %d: Connecting to Wi-Fi...\n", attempt);
        int result = cyw43_arch_wifi_connect_timeout_ms(
            ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 15000
        );

        if (result == 0) {
            uint8_t *ip = (uint8_t*)&cyw43_state.netif[0].ip_addr.addr;
            printf("Connected to Wi-Fi on attempt %d! IP: %d.%d.%d.%d\n", attempt, ip[0], ip[1], ip[2], ip[3]);
            return true;
        } else {
            printf("Wi-Fi connection failed (code %d). Retrying in %d ms...\n", result, WIFI_RETRY_DELAY_MS);
            sleep_ms(WIFI_RETRY_DELAY_MS);
        }
    }

    printf("All Wi-Fi connection attempts failed.\n");
    return false;
}
