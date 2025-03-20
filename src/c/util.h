#include <pebble.h>

#ifndef UTIL_H
#define UTIL_H

BatteryChargeState get_current_battery_info();
int get_current_charge_percent();
int get_current_step_count();
int get_length_of_month(int month);

#endif