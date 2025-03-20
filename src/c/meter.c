#include "meter.h"
#include "util.h"
#include "quad_math.h"

// BMC TODO Parameterize the maximum step amount.

extern int dom, month;

void redraw_meters(GContext *context) {
    int battery_meter_angle = TRIG_MAX_ANGLE * (get_current_charge_percent() / 100.0f);
    graphics_context_set_fill_color(context, GColorScreaminGreen);
    graphics_fill_radial(context, 
                         center_obj_in_quadrant(BOTTOM_LEFT, GSize(BATTERY_METER_SIZE, BATTERY_METER_SIZE)), 
                         GOvalScaleModeFitCircle, 
                         METER_WIDTH, 
                         0, 
                         battery_meter_angle);

    graphics_context_set_stroke_color(context, GColorScreaminGreen);
    graphics_draw_circle(context, get_quadrant_center(BOTTOM_LEFT), (BATTERY_METER_SIZE  / 2) - METER_WIDTH);
    
    int step_meter_angle = TRIG_MAX_ANGLE * (get_current_step_count() / 8000.0f);
    graphics_context_set_fill_color(context, GColorVeryLightBlue);
    graphics_fill_radial(context, 
                         center_obj_in_quadrant(BOTTOM_LEFT, GSize(STEP_METER_SIZE, STEP_METER_SIZE)), 
                         GOvalScaleModeFillCircle, 
                         METER_WIDTH, 
                         0, 
                         step_meter_angle);
                         
    graphics_context_set_stroke_color(context, GColorVeryLightBlue);
    graphics_draw_circle(context, get_quadrant_center(BOTTOM_LEFT), (STEP_METER_SIZE  / 2) - METER_WIDTH);
}