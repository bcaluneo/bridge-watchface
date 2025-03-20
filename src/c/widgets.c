#include "widgets.h"
#include "quad_math.h"
#include "util.h"

static TextLayer *charge_count, *step_count, *date, *dow;
static GDrawCommandImage *steps_image, *calendar_image;
static GPath *bolt_path = NULL;
static GSize steps_image_bounds, calendar_image_bounds;

static char charge_buffer[8], step_buffer[8], date_buffer[16], dow_buffer[8];

void widgets_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
 
    // just some bullshit for now
    int h_w = 15;
    int charge_offset = 10;

    calendar_image = gdraw_command_image_create_with_resource(RESOURCE_ID_CALENDAR);
    steps_image = gdraw_command_image_create_with_resource(RESOURCE_ID_STEPS);

    steps_image_bounds = gdraw_command_image_get_bounds_size(steps_image);
    calendar_image_bounds = gdraw_command_image_get_bounds_size(calendar_image);

    step_count = text_layer_create(GRect(DISPLAY_WIDTH_HALF + steps_image_bounds.w, DISPLAY_HEIGHT_HALF, bounds.size.w, 50)); 
    charge_count = text_layer_create(GRect(DISPLAY_WIDTH_HALF + steps_image_bounds.w, DISPLAY_HEIGHT_HALF + steps_image_bounds.h, bounds.size.w, 50)); 
    date = text_layer_create(GRect(DISPLAY_WIDTH_HALF + steps_image_bounds.w, DISPLAY_HEIGHT_HALF + (steps_image_bounds.h * 2) + 10, bounds.size.w, 50));
    dow = text_layer_create(GRect(DISPLAY_WIDTH_HALF + steps_image_bounds.w, DISPLAY_HEIGHT_HALF + (steps_image_bounds.h * 2) - 5, bounds.size.w, 50));

    text_layer_set_background_color(dow, GColorClear);
    text_layer_set_text_color(dow, GColorLavenderIndigo);
    text_layer_set_font(dow, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
    text_layer_set_text_alignment(dow, GTextAlignmentLeft);  

    text_layer_set_background_color(date, GColorClear);
    text_layer_set_text_color(date, GColorLavenderIndigo);
    text_layer_set_font(date, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
    text_layer_set_text_alignment(date, GTextAlignmentLeft);  
  
    text_layer_set_background_color(step_count, GColorClear);
    text_layer_set_text_color(step_count, GColorVeryLightBlue);
    text_layer_set_font(step_count, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
    text_layer_set_text_alignment(step_count, GTextAlignmentLeft);
  
    text_layer_set_background_color(charge_count, GColorClear);
    text_layer_set_text_color(charge_count, GColorScreaminGreen);
    text_layer_set_font(charge_count, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
    text_layer_set_text_alignment(charge_count, GTextAlignmentLeft);

    bolt_path = gpath_create(&BOLT_PATH_INFO);

    GPoint quad_center = get_quadrant_center(BOTTOM_LEFT);
    GRect bolt_box = center_obj_in_quadrant(BOTTOM_LEFT, BOLT_PATH_BOUNDS);
    bolt_path->offset = GPoint(DISPLAY_WIDTH_HALF + 5, DISPLAY_HEIGHT_HALF + steps_image_bounds.h - 2);

    time_t now = time(NULL);
    tick_widgets(localtime(&now));
    
    layer_add_child(window_layer, text_layer_get_layer(dow));
    layer_add_child(window_layer, text_layer_get_layer(step_count));
    layer_add_child(window_layer, text_layer_get_layer(charge_count));
    layer_add_child(window_layer, text_layer_get_layer(date));
}

void widgets_unload() {
    gpath_destroy(bolt_path);
    gdraw_command_image_destroy(steps_image);
    gdraw_command_image_destroy(calendar_image);

    text_layer_destroy(charge_count);
    text_layer_destroy(step_count);
    text_layer_destroy(date);
}

void tick_widgets(struct tm *local_time) {
    snprintf(step_buffer, sizeof(step_buffer), "%d", get_current_step_count());

    BatteryChargeState current_battery_state = get_current_battery_info();
    if (!current_battery_state.is_charging) {
        snprintf(charge_buffer, sizeof(charge_buffer), "%d%%", get_current_battery_info().charge_percent);
    } else {
        snprintf(charge_buffer, sizeof(charge_buffer), "%s", "CHRG");
    }

    if (local_time) {
        strftime(date_buffer, sizeof(date_buffer), "%b %e", local_time);
        strftime(dow_buffer, sizeof(date_buffer), "%a.", local_time);
    }

    text_layer_set_text(dow, dow_buffer);
    text_layer_set_text(step_count, step_buffer);
    text_layer_set_text(charge_count, charge_buffer);
    text_layer_set_text(date, date_buffer); 
}

void redraw_widgets(GContext *context) {
    // Steps
    gdraw_command_image_draw(context, steps_image, GPoint(DISPLAY_WIDTH_HALF, DISPLAY_HEIGHT_HALF));

    // Battery
    graphics_context_set_fill_color(context, GColorScreaminGreen);
    gpath_draw_filled(context, bolt_path);

    // Calendar
    gdraw_command_image_draw(context, calendar_image, GPoint(DISPLAY_WIDTH_HALF, DISPLAY_HEIGHT_HALF + (steps_image_bounds.h * 2)));
}