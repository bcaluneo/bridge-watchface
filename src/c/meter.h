#include <pebble.h>

#ifndef METER_H
#define METER_H

#define BATTERY_METER_SIZE 50
#define STEP_METER_SIZE 30
#define METER_WIDTH 5

void redraw_meters(GContext *context);

#endif