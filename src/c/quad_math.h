#include <pebble.h>

#ifndef QUAD_MATH_H
#define QUAD_MATH_H

#define DISPLAY_WIDTH_HALF PBL_DISPLAY_WIDTH/2
#define DISPLAY_HEIGHT_HALF PBL_DISPLAY_HEIGHT/2

#define BASE_CENTER_X DISPLAY_WIDTH_HALF/2
#define BASE_CENTER_Y DISPLAY_HEIGHT_HALF/2

typedef enum {
    TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT
}  Quadrant;

GPoint get_quadrant_center(const Quadrant quadrant);
GRect center_obj_in_quadrant(const Quadrant quadrant, const GSize bounds);

#endif