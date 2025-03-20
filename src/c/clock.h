#include <pebble.h>

#ifndef CLOCK_H
#define CLOCK_H

void clock_load(Window *window);
void clock_unload();

void tick_clock();

#endif