#include "telemetry.h"
#include <stdio.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <math.h>

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

    if (!cJSON_IsObject(root))
    {
        printf("Invalid JSON object!\n");
        cJSON_Delete(root);
        return 0;
    }

    cJSON *deviceId = cJSON_GetObjectItemCaseSensitive(root, "device_id");
    if (deviceId == NULL ||
        !cJSON_IsString(deviceId) ||
        deviceId->valuestring[0] == '\0' ||
        strlen(deviceId->valuestring) >= DEVICE_ID_LENGTH)
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
    if (humidity == NULL ||
        !cJSON_IsNumber(humidity) ||
        humidity->valuedouble > 100 ||
        humidity->valuedouble < 0)
    {
        printf("Invalid humidity!\n");
        cJSON_Delete(root);
        return 0;
    }

    cJSON *battery = cJSON_GetObjectItemCaseSensitive(root, "battery");
    if (battery == NULL ||
        !cJSON_IsNumber(battery) ||
        battery->valuedouble > 100 ||
        battery->valuedouble < 0 ||
        battery->valuedouble != (double)battery->valueint)
    {
        printf("Invalid battery!\n");
        cJSON_Delete(root);
        return 0;
    }

    cJSON *timestamp = cJSON_GetObjectItemCaseSensitive(root, "timestamp");
    if (timestamp == NULL ||
        !cJSON_IsNumber(timestamp) ||
        !isfinite(timestamp->valuedouble) ||
        timestamp->valuedouble < 0 ||
        timestamp->valuedouble >= 9223372036854775808.0 ||
        timestamp->valuedouble != (double)(long long)timestamp->valuedouble)
    {
        printf("Invalid timestamp!\n");
        cJSON_Delete(root);
        return 0;
    }

    float temperatureValue = temperature->valuedouble;
    float humidityValue = humidity->valuedouble;
    int batteryValue = battery->valueint;
    long long timestampValue = timestamp->valuedouble;

    Device *device = device_store_find_or_add(store, deviceId->valuestring);
    if (device == NULL)
    {
        printf("Failed to find or create device!\n");
        cJSON_Delete(root);
        return 0;
    }

    device_update(device, temperatureValue, humidityValue, batteryValue, timestampValue);
    telemetry_check_alerts(device);

    cJSON_Delete(root);
    return 1;
}

void telemetry_check_alerts(const Device *device)
{
    if (device == NULL)
    {
        return;
    }

    if (device->temperature > 30)
    {
        printf("HIGH TEMPERATURE!\n");
        printf("Device: %s\n", device->id);
        printf("Temperature: %.1f C\n", device->temperature);
    }

    if (device->humidity > 80)
    {
        printf("HIGH HUMIDITY!\n");
        printf("Device: %s\n", device->id);
        printf("Humidity: %.1f %%\n", device->humidity);
    }

    if (device->battery < 20)
    {
        printf("BATTERY TOO LOW!\n");
        printf("Device: %s\n", device->id);
        printf("Battery: %d %%\n", device->battery);
    }
}