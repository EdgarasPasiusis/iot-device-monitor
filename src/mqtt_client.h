#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "device.h"

struct mosquitto;

typedef struct
{
    struct mosquitto *mosq;
    DeviceStore *store;
} MqttClient;

int mqtt_client_start(MqttClient *client, DeviceStore *store);
void mqtt_client_stop(MqttClient *client);

#endif