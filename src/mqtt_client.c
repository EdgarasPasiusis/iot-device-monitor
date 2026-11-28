#include "mqtt_client.h"
#include <stdio.h>
#include <mosquitto.h>
#include <stdbool.h>
#include "telemetry.h"
#include <stdlib.h>
#include <string.h>

void on_message(
    struct mosquitto *mosq,
    void *userdata,
    const struct mosquitto_message *message)
{
    (void)mosq;

    MqttClient *client = userdata;
    DeviceStore *store = client->store;
    (void)store;

    if (message == NULL || message->payload == NULL)
    {
        return;
    }

    printf("Received MQTT message!\n");
    printf("Topic: %s\n", message->topic);
}

void on_connect(struct mosquitto *mosq, void *userdata, int result)
{
    (void)userdata;

    if (result != 0)
    {
        printf("Failed to connect to MQTT broker! Error: %d\n", result);
        return;
    }

    printf("Connected to MQTT broker successfully!\n");

    int subscribe_result = mosquitto_subscribe(
        mosq,
        NULL,
        "iot/devices/+/telemetry",
        0);

    if (subscribe_result != MOSQ_ERR_SUCCESS)
    {
        printf("Failed to subscribe to MQTT topic!\n");
        return;
    }
    printf("Subscription request sent succesfully!\n");
}

int mqtt_client_start(MqttClient *client, DeviceStore *store)
{
    if (store == NULL || client == NULL)
    {
        printf("Invalid MQTT client or device store!\n");
        return 0;
    }

    client->store = store;
    client->mosq = NULL;
    printf("Starting MQTT client ...\n");

    int result = mosquitto_lib_init();

    if (result != MOSQ_ERR_SUCCESS)
    {
        printf("MQTT library initialization failed!\n");
        return 0;
    }
    printf("MQTT library initialized successfully!\n");

    client->mosq = mosquitto_new(
        "iot_device_monitor",
        true,
        client);

    if (client->mosq == NULL)
    {
        printf("Failed to create MQTT client!\n");
        mosquitto_lib_cleanup();
        return 0;
    }
    printf("MQTT client created successfully!\n");

    mosquitto_message_callback_set(client->mosq, on_message);
    mosquitto_connect_callback_set(client->mosq, on_connect);

    result = mosquitto_connect(client->mosq, "localhost", 1883, 60);
    if (result != MOSQ_ERR_SUCCESS)
    {
        printf("Failed to connect to MQTT broker!\n");
        mosquitto_destroy(client->mosq);
        mosquitto_lib_cleanup();
        return 0;
    }

    result = mosquitto_loop_start(client->mosq);

    if (result != MOSQ_ERR_SUCCESS)
    {
        printf("Failed to start MQTT network loop!\n");
        mosquitto_destroy(client->mosq);
        client->mosq = NULL;
        mosquitto_lib_cleanup();
        return 0;
    }

    printf("MQTT network loop started successfully!\n");

    return 1;
}

void mqtt_client_stop(MqttClient *client)
{
    if (client == NULL || client->mosq == NULL)
    {
        return;
    }
    printf("Stopping MQTT client...\n");

    mosquitto_disconnect(client->mosq);
    mosquitto_loop_stop(client->mosq, false);
    mosquitto_destroy(client->mosq);
    client->mosq = NULL;
    mosquitto_lib_cleanup();
    printf("MQTT client stopped successfully!\n");
}