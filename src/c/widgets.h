#include <pebble.h>

#ifndef WIDGETS_H
#define WIDGETS_H

// This lightning bolt was shamelessly taken from the Pebble Developer Wiki.

static const GPathInfo BOLT_PATH_INFO = {
  .num_points = 6,
  .points = (GPoint []) {
    {10, 0}, {7, 13}, {14, 13}, {3, 30}, {7, 17}, {0, 17}
  }
};

static const GSize BOLT_PATH_BOUNDS = {
  .w = 14,
  .h = 30
};

void widgets_load();
void widgets_unload();

void tick_widgets(struct tm *local_time);
void redraw_widgets(GContext *context);

#endif