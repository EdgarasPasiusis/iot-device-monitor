#include "telemetry.h"
#include <stdio.h>
#include <string.h>
#include <cjson/cJSON.h>

int telemetry_process(DeviceStore *store, const char *json)
{
    if (store == NULL || json == NULL)
    {
        printf("Invalid input!\n");
        return 0;
    }

    cJSON *root = cJSON_Parse(json);

    if (root == NULL)
    {
        printf("JSON parsing failed!\n");
        return 0;
    }

    cJSON *deviceId = cJSON_GetObjectItemCaseSensitive(root, "device_id");

    if (deviceId == NULL || !cJSON_IsString(deviceId) || deviceId->valuestring[0] == '\0' || strlen(deviceId->valuestring) >= DEVICE_ID_LENGTH)
    {
        printf("Invalid device ID!\n");
        cJSON_Delete(root);
        return 0;
    }

    cJSON *temperature = cJSON_GetObjectItemCaseSensitive(root, "temperature");

    if (temperature == NULL || !cJSON_IsNumber(temperature))
    {
        printf("Invalid temperature!\n");
        cJSON_Delete(root);
        return 0;
    }

    cJSON *humidity = cJSON_GetObjectItemCaseSensitive(root, "humidity");

    if (humidity == NULL || !cJSON_IsNumber(humidity))
    {
        printf("Invalid humidity!\n");
        cJSON_Delete(root);
        return 0;
    }

    cJSON *battery = cJSON_GetObjectItemCaseSensitive(root, "battery");

    if (battery == NULL || !cJSON_IsNumber(battery))
    {
        printf("Invalid battery!\n");
        cJSON_Delete(root);
        return 0;
    }

    cJSON *timestamp = cJSON_GetObjectItemCaseSensitive(root, "timestamp");

    if (timestamp == NULL || !cJSON_IsNumber(timestamp))
    {
        printf("Invalid timestamp!\n");
        cJSON_Delete(root);
        return 0;
    }

    cJSON_Delete(root);
    return 1;
}