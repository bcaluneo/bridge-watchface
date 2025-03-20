#include "clock.h"

static BitmapLayer *track_layer;
static BitmapLayer *first_moon_layer;
static BitmapLayer *second_moon_layer;
static BitmapLayer *sun_layer;
static BitmapLayer *third_moon_layer;
static BitmapLayer *fifth_moon_layer;

static GBitmap *track_bitmap, *reverse_moon_bitmap, *forward_moon_bitmap, *sun_bitmap;

static TextLayer *clock_time, *clock_suffix, *dst;
static char time_buffer[8], time_suffix_buffer[4], dst_buffer[4];

void clock_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    reverse_moon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_REVERSE_MOON);
    forward_moon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FORWARD_MOON);
    sun_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SUN);
    track_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TRACK);

    track_layer = bitmap_layer_create(GRect(10, 64, 124, 10));
    first_moon_layer = bitmap_layer_create(GRect(5, 60, 24, 24));
    second_moon_layer = bitmap_layer_create(GRect(30, 55, 24, 24));
    sun_layer = bitmap_layer_create(GRect(55, 50, 32, 32));
    third_moon_layer = bitmap_layer_create(GRect(87, 55, 24, 24));
    fifth_moon_layer = bitmap_layer_create(GRect(112, 60, 24, 24));

    bitmap_layer_set_compositing_mode(first_moon_layer, GCompOpSet);
    bitmap_layer_set_compositing_mode(second_moon_layer, GCompOpSet);
    bitmap_layer_set_compositing_mode(sun_layer, GCompOpSet);
    bitmap_layer_set_compositing_mode(third_moon_layer, GCompOpSet);
    bitmap_layer_set_compositing_mode(fifth_moon_layer, GCompOpSet);

    bitmap_layer_set_bitmap(track_layer, track_bitmap);

    clock_time = text_layer_create(GRect(0, 0, bounds.size.w - 35, 50));
    clock_suffix = text_layer_create(GRect(bounds.size.w - 30, 0, bounds.size.w, 50));
    dst = text_layer_create(GRect(bounds.size.w - 30, 25, bounds.size.w, 50));
  
    text_layer_set_background_color(clock_time, GColorClear);
    text_layer_set_text_color(clock_time, GColorWhite);
    text_layer_set_font(clock_time, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
    text_layer_set_text_alignment(clock_time, GTextAlignmentCenter);
  
    text_layer_set_background_color(clock_suffix, GColorClear);
    text_layer_set_text_color(clock_suffix, GColorWhite);
    text_layer_set_font(clock_suffix, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text_alignment(clock_suffix, GTextAlignmentLeft);
  
    text_layer_set_background_color(dst, GColorClear);
    text_layer_set_text_color(dst, GColorWhite);
    text_layer_set_font(dst, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text_alignment(dst, GTextAlignmentLeft);

    layer_add_child(window_layer, text_layer_get_layer(clock_time));
    layer_add_child(window_layer, text_layer_get_layer(clock_suffix));
    layer_add_child(window_layer, text_layer_get_layer(dst));

    layer_add_child(window_layer, bitmap_layer_get_layer(track_layer));
    layer_add_child(window_layer, bitmap_layer_get_layer(first_moon_layer));
    layer_add_child(window_layer, bitmap_layer_get_layer(second_moon_layer));
    layer_add_child(window_layer, bitmap_layer_get_layer(sun_layer));
    layer_add_child(window_layer, bitmap_layer_get_layer(third_moon_layer));
    layer_add_child(window_layer, bitmap_layer_get_layer(fifth_moon_layer));

    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    tick_clock(local_time);
}

void tick_clock(struct tm *local_time) {
    if (local_time->tm_min == 0) vibes_double_pulse();
  
    strftime(time_buffer, sizeof(time_buffer), "%I:%M", local_time);
  
    // Remove the trailing 0 that strftime places.
    if ((local_time->tm_hour >= 1 && local_time->tm_hour < 10) || (local_time->tm_hour >= 13 && local_time->tm_hour < 22)) {
      strncpy(time_buffer, time_buffer + 1, sizeof(time_buffer) - 1);
    }
  
    text_layer_set_text(clock_time, time_buffer);
  
    snprintf(time_suffix_buffer, sizeof(time_suffix_buffer), "%s", (local_time->tm_hour < 12) ? "AM" : "PM");
    text_layer_set_text(clock_suffix, time_suffix_buffer);
  
    snprintf(dst_buffer, sizeof(dst_buffer), "%s", local_time->tm_isdst > 0 ? "DST" : "");
    text_layer_set_text(dst, dst_buffer);

    int current_hour = local_time->tm_hour;

    // For testing purposes...
    // bitmap_layer_set_bitmap(first_moon_layer, reverse_moon_bitmap);
    // bitmap_layer_set_bitmap(second_moon_layer, reverse_moon_bitmap);
    // bitmap_layer_set_bitmap(sun_layer, sun_bitmap);
    // bitmap_layer_set_bitmap(third_moon_layer, forward_moon_bitmap);
    // bitmap_layer_set_bitmap(fifth_moon_layer, forward_moon_bitmap);

    bitmap_layer_set_bitmap(first_moon_layer, current_hour < 3 ? reverse_moon_bitmap : NULL);
    bitmap_layer_set_bitmap(second_moon_layer, (current_hour >= 3 && current_hour < 6) ? reverse_moon_bitmap : NULL);
    bitmap_layer_set_bitmap(sun_layer, (current_hour >= 6 && current_hour < 18) ? sun_bitmap : NULL);
    bitmap_layer_set_bitmap(third_moon_layer, (current_hour >= 18 && current_hour < 21) ? forward_moon_bitmap : NULL);
    bitmap_layer_set_bitmap(fifth_moon_layer, current_hour >= 21 ? forward_moon_bitmap : NULL);
}

void clock_unload() {
    gbitmap_destroy(reverse_moon_bitmap);
    gbitmap_destroy(forward_moon_bitmap);
    gbitmap_destroy(sun_bitmap);
    gbitmap_destroy(track_bitmap);

    bitmap_layer_destroy(track_layer);
    bitmap_layer_destroy(first_moon_layer);
    bitmap_layer_destroy(second_moon_layer);
    bitmap_layer_destroy(sun_layer);
    bitmap_layer_destroy(third_moon_layer);
    bitmap_layer_destroy(fifth_moon_layer);

    text_layer_destroy(clock_time);
    text_layer_destroy(clock_suffix);
    text_layer_destroy(dst);
}