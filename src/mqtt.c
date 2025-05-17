#include <stdio.h>
#include <string.h>
#include "mqtt.h"
#include <stdbool.h>

volatile bool mqtt_connected = false;
volatile bool alarm_enabled = true;
volatile bool alarm_synced = false;



static const struct mqtt_connect_client_info_t mqtt_client_info = {
    .client_id   = "pi",
    .client_user = NULL,
    .client_pass = NULL,
    .keep_alive  = 30,
    .will_topic  = "device/status",
    .will_msg    = "offline",
    .will_qos    = 1,
    .will_retain = 0
};

static void mqtt_publish_cb(void *arg, err_t result) {
    if (result == ERR_OK) {
        printf("Publish successful.\n");
    } else {
        printf("Publish failed with error %d.\n", result);
    }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("MQTT connected successfully.\n");
        mqtt_connected = true;

        err_t err = mqtt_subscribe(client, "device/alarm", 1, NULL, NULL);
        if (err != ERR_OK) {
            printf("❌ Failed to subscribe to device/alarm: %d\n", err);
        } else {
            printf("✅ Subscribed to device/alarm topic\n");
        }

        // Request the current alarm state from the server
        const char *sync_topic = "device/alarm/request";
        const char *sync_msg = "request";
        mqtt_publish(client, sync_topic, sync_msg, strlen(sync_msg), 1, 0, mqtt_publish_cb, NULL);
        printf("Requested alarm state sync from server.\n");

        // Publish initial online message
        const char *initial_topic = "device/status";
        const char *initial_msg = "online";
        mqtt_publish(client, initial_topic, initial_msg, strlen(initial_msg), 1, 0, mqtt_publish_cb, NULL);
        printf("Initial connection message published to '%s'\n", initial_topic);
    } else {
        printf("MQTT connection failed with status %d.\n", status);
        mqtt_connected = false;
    }
}

static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
    printf("Incoming message on topic: %s\n", topic);
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    printf("🔥 mqtt_incoming_data_cb triggered! len = %d\n", len);
    
    if (data && len > 0) {
        char msg[32] = {0};
        strncpy(msg, (const char*)data, len < sizeof(msg) ? len : sizeof(msg)-1);
        msg[len] = '\0';

        printf("Received payload: %s\n", msg);

        if (strcmp(msg, "off") == 0) {
            alarm_enabled = false;
            alarm_synced = true;
            printf("Alarm disabled via MQTT (sync or user command)\n");
        } else if (strcmp(msg, "on") == 0) {
            alarm_enabled = true;
            alarm_synced = true;
            printf("Alarm enabled via MQTT (sync or user command)\n");
        }
    }
}

void setup_mqtt_client(mqtt_client_t **client) {
    ip_addr_t broker_ip;
    IP4_ADDR(&broker_ip, 192, 168, 1, 70);

    *client = mqtt_client_new();
    if (*client == NULL) {
        printf("Failed to create MQTT client\n");
        return;
    }

    mqtt_client_connect(*client, &broker_ip, 1883, mqtt_connection_cb, NULL, &mqtt_client_info);
    mqtt_set_inpub_callback(*client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, NULL);
}

void publish_distance(mqtt_client_t *client, float distance) {
    if (!mqtt_connected) {
        printf("MQTT client not connected\n");
        return;
    }

    char payload[64];
    snprintf(payload, sizeof(payload), "%.2f", distance);
    mqtt_publish(client, "motion/distance", payload, strlen(payload), 0, 0, mqtt_publish_cb, NULL);
}

