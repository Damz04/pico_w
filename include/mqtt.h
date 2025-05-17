#ifndef MQTT_HELPER_H
#define MQTT_HELPER_H

#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"
#include <stdbool.h>

extern volatile bool mqtt_connected;
extern volatile bool alarm_enabled;
extern volatile bool alarm_synced;

void setup_mqtt_client(mqtt_client_t **client);
void publish_distance(mqtt_client_t *client, float distance);

#endif
