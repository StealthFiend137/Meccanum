#pragma once
#include "pico/time.h"
#include "movement.h"

/// @brief Sets all of the speeds for this movement object.  This restarts all of the relevent timeout timers.
/// @param x X Axis.
/// @param y Y Axis.
/// @param w W Axis.
void Movement::set_all_speeds(int x, int y, int w)
{
    int start_time = to_ms_since_boot(get_absolute_time());
    xAxis.set_speed(x, start_time);
    yAxis.set_speed(y, start_time);
    wAxis.set_speed(w, start_time);
};
