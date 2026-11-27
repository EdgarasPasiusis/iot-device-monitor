#include "mqtt_client.h"
#include <stdio.h>
#include <mosquitto.h>
#include <stdbool.h>

void on_message(
    struct mosquitto *mosq,
    void *userdata,
    const struct mosquitto_message *message)
{
    (void)mosq;

    DeviceStore *store = userdata;
    (void)store;

    if (message == NULL || message->payload == NULL)
    {
        return;
    }

    printf("Received MQTT message!\n");
    printf("Topic: %s\n", message->topic);
}

int mqtt_client_start(DeviceStore *store)
{
    if (store == NULL)
    {
        printf("Invalid device store!\n");
        return 0;
    }

    printf("Starting MQTT client ...\n");

    int result = mosquitto_lib_init();

    if (result != MOSQ_ERR_SUCCESS)
    {
        printf("MQTT library initialization failed!\n");
        return 0;
    }
    printf("MQTT library initialized successfully!\n");

    struct mosquitto *mosq = mosquitto_new(
        "iot_device_monitor",
        true,
        store);

    if (mosq == NULL)
    {
        printf("Failed to create MQTT client!\n");
        return 0;
    }
    printf("MQTT client created successfully!\n");

    mosquitto_message_callback_set(mosq, on_message);

    result = mosquitto_connect(mosq, "localhost", 1883, 60);
    if (result != MOSQ_ERR_SUCCESS)
    {
        printf("Failed to connect to MQTT broker!\n");
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 0;
    }
    printf("Connected to MQTT broker successfully!\n");

    result = mosquitto_subscribe(mosq, NULL, "iot/devices/+/telemetry", 0);
    if (result != MOSQ_ERR_SUCCESS)
    {
        printf("Failed to subscribe to MQTT topic!\n");
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 0;
    }
    printf("Subscribed to telemetry topic successfully!\n");

    return 1;
}