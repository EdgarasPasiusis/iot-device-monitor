#ifndef DEVICE_H
#define DEVICE_H

#include <stddef.h>

#define DEVICE_ID_LENGTH 32
#define MAX_DEVICES 100

typedef struct
{
    char id[DEVICE_ID_LENGTH];
    float temperature;
    float humidity;
    int battery;
    long long timestamp;
} Device;

typedef struct
{
    Device devices[MAX_DEVICES];
    size_t count;
} DeviceStore;

Device *device_store_find_or_add(DeviceStore *store, const char *id);

void device_print(const Device *device);

void device_update(
    Device *device,
    float temperature,
    float humidity,
    int battery,
    long long timestamp
);

#endif