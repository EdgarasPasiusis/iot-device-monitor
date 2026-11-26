#include "device.h"

#include <stdio.h>
#include <string.h>

Device *device_store_find_or_add(DeviceStore *store, const char *id)
{
    if (store == NULL || id == NULL || id[0] == '\0')
    {
        return NULL;
    }

    if (strlen(id) >= DEVICE_ID_LENGTH)
    {
        return NULL;
    }

    for (size_t i = 0; i < store->count; i++)
    {
        if (strcmp(store->devices[i].id, id) == 0)
        {
            return &store->devices[i];
        }
    }

    if (store->count >= MAX_DEVICES)
    {
        return NULL;
    }

    Device *device = &store->devices[store->count];

    memset(device, 0, sizeof(*device));
    strcpy(device->id, id);

    store->count++;

    return device;
}

void device_print(const Device *device)
{
    if (device == NULL)
    {
        return;
    }

    printf("Device: %s\n", device->id);
    printf("Temperature: %.1f C\n", device->temperature);
    printf("Humidity: %.1f %%\n", device->humidity);
    printf("Battery: %d %%\n", device->battery);
    printf("Timestamp: %lld\n", device->timestamp);
}

void device_update(
    Device *device,
    float temperature,
    float humidity,
    int battery,
    long long timestamp)
{
    if (device == NULL)
    {
        return;
    }
    device->temperature = temperature;
    device->humidity = humidity;
    device->battery = battery;
    device->timestamp = timestamp;
}