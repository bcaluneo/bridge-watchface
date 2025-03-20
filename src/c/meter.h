#include <pebble.h>

#ifndef METER_H
#define METER_H

#define BATTERY_METER_SIZE 55
#define STEP_METER_SIZE 35
#define METER_WIDTH 5

void redraw_meters(GContext *context);

#endif