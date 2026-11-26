#include <stdio.h>
#include "device.h"

int main(void)
{
    DeviceStore store = {0};

    Device *device1 = device_store_find_or_add(&store, "sensor_01");

    device_update(device1, 25.5, 60.0, 85, 1794301200);

    Device *device2 = device_store_find_or_add(&store, "sensor_02");
    Device *device3 = device_store_find_or_add(&store, "sensor_01");

    printf("Total devices: %zu\n", store.count);

    device_print(device1);
    device_print(device2);
    device_print(device3);

    return 0;
}