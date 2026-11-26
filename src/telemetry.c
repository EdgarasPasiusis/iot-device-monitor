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
    cJSON_Delete(root); /*Geriau trinti atminti po visa json apdorojimo*/
    return 1;
}