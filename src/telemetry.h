#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "device.h"

int telemetry_process(DeviceStore *store, const char *json);

void telemetry_check_alerts(const Device *device);

#endif