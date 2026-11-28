#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "device.h"
#include "mqtt_client.h"

static volatile sig_atomic_t running = 1;

void handle_sigint(int signal_number)
{
    (void)signal_number;
    running = 0;
}

int main(void)
{
    DeviceStore store = {0};
    MqttClient client = {0};

    signal(SIGINT, handle_sigint);

    printf("Starting IoT Device Monitor...\n");

    if (!mqtt_client_start(&client, &store))
    {
        printf("Failed to start MQTT client!\n");
        return 1;
    }

    printf("IoT Device Monitor is running!\n");
    printf("Press Ctrl+C to stop.\n");

    while (running)
    {
        sleep(1);
    }

    printf("\nShutdown signal received!\n");

    mqtt_client_stop(&client);

    printf("IoT Device Monitor stopped.\n");

    return 0;
}