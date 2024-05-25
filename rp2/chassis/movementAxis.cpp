#pragma once
#include "movementAxis.h"
#include <pico/time.h>

/// @brief Sets the speed of the axis and the time to set as the start point.
/// @param speed The speed of the axis.
/// @param start_time The start time.
void MovementAxis::set_speed(int speed, int start_time)
{
    _speed = speed;
    StartTime = start_time;
};

/// @brief Sets the speed of the axis.
/// @param speed The speed of the axis.
void MovementAxis::set_speed(int speed)
{
    int start_time = to_ms_since_boot(get_absolute_time());
    set_speed(speed, start_time);
};

/// @brief Gets the speed of the axis.
/// @param void 
/// @return Returns the speed of the axis.
int MovementAxis::get_speed(void)
{
    ChangedSinceLastRead = false;
    return _speed;
};
