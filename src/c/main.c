#include <pebble.h>

#include "util.h"
#include "quad_math.h"
#include "clock.h"
#include "meter.h"
#include "widgets.h"

static Window *window;
static Layer *canvas;

static void battery_state_handler(BatteryChargeState charge) {
  time_t now = time(NULL);
  tick_widgets(localtime(&now));

  layer_mark_dirty(canvas);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  time_t now = time(NULL);
  struct tm *local_time = localtime(&now);

  tick_widgets(local_time);
  tick_clock(local_time);

  layer_mark_dirty(canvas);
}

static void update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  redraw_meters(ctx);
  redraw_widgets(ctx);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  canvas = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_set_update_proc(canvas, update_proc);

  layer_add_child(window_layer, canvas);

  widgets_load(window);
  clock_load(window);
}

static void window_unload(Window *window) {
  layer_destroy(canvas);
  widgets_unload();
  clock_unload();
}

static void start(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Bridge Watchface has started.");

  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(window, true);
  window_set_background_color(window, GColorClear);

  time_t now = time(NULL);
  struct tm *local_time = localtime(&now);
  tick_clock(local_time);
}

static void finish(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Bridge Watchface has finished.");
  battery_state_service_unsubscribe();
  window_destroy(window);
}

int main(void) {
  start();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(battery_state_handler);

  app_event_loop();
  finish();
}
