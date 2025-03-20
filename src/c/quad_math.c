#include "quad_math.h"

GPoint get_quadrant_center(const Quadrant quadrant) {
    int x_off = 0;
    int y_off = 0;

    switch (quadrant) {
        case 1: // TOP_RIGHT
            x_off += DISPLAY_WIDTH_HALF;
            break;
        case 2: // BOTTOM_LEFT
            y_off += DISPLAY_HEIGHT_HALF;
        break;
        case 3: // BOTTOM_RIGHT
            x_off += DISPLAY_WIDTH_HALF;
            y_off += DISPLAY_HEIGHT_HALF;
        break;
        default: // TOP_LEFT
            break;
    }

    return GPoint(x_off + BASE_CENTER_X, y_off + BASE_CENTER_Y);
}

GRect center_obj_in_quadrant(const Quadrant quadrant, const GSize bounds) {
    GPoint quadrant_center = get_quadrant_center(quadrant);
    int bounds_w = bounds.w;
    int bounds_h = bounds.h;

    return GRect(quadrant_center.x - (bounds_w / 2), quadrant_center.y - (bounds_h / 2), bounds_w, bounds_h);
}