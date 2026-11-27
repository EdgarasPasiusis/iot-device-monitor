#include <stdio.h>
#include "device.h"
#include "telemetry.h"

int main(void)
{
    DeviceStore store = {0};

    const char *json1 =
        "{"
        "\"device_id\":\"sensor_01\","
        "\"temperature\":25.5,"
        "\"humidity\":60.0,"
        "\"battery\":85,"
        "\"timestamp\":1794301200"
        "}";

    const char *json2 =
        "{"
        "\"device_id\":\"sensor_02\","
        "\"temperature\":35.0,"
        "\"humidity\":90.0,"
        "\"battery\":15,"
        "\"timestamp\":1794301300"
        "}";

    const char *json3 =
        "{"
        "\"device_id\":\"sensor_01\","
        "\"temperature\":29.5,"
        "\"humidity\":70.0,"
        "\"battery\":75,"
        "\"timestamp\":1794301400"
        "}";

    const char *json4 =
        "{"
        "\"device_id\":\"sensor_03\","
        "\"temperature\":25.0,"
        "\"humidity\":50.0,"
        "\"battery\":150,"
        "\"timestamp\":1794301500"
        "}";

    const char *json5 =
        "{"
        "\"device_id\":\"sensor_03\","
        "\"temperature\":25.0,"
        "\"humidity\":50.0,"
        "\"timestamp\":1794301600.5"
        "}";

    const char *json6 = "[\"sensor_01\", \"sensor_02\"]";

    telemetry_process(&store, json1);
    telemetry_process(&store, json2);
    telemetry_process(&store, json3);
    telemetry_process(&store, json4);
    telemetry_process(&store, json5);
    telemetry_process(&store, json6);

    printf("Total devices: %zu\n", store.count);

    for (size_t i = 0; i < store.count; i++)
    {
        device_print(&store.devices[i]);
    }

    return 0;
}