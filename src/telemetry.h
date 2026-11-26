#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "device.h"

int telemetry_process(DeviceStore *store, const char *json);

#endif