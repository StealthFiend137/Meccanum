#include <pico/time.h>
#include "movementAxis.h"

/// @brief Sets the speed of the axis and the time to set as the start point.
/// @param speed The speed of the axis.
/// @param start_time The start time.
void MovementAxis::set_speed(int speed, int start_time)
{
    _velocity = std::clamp(speed, -100, 100);
    StartTime = start_time;
};

/// @brief Gets the speed of the axis.
/// @param void 
/// @return Returns the speed of the axis.
int MovementAxis::get_speed(void)
{
    ChangedSinceLastRead = false;
    return _velocity;
};
