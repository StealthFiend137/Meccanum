#pragma once
#include "movementAxis.h"

/// @brief Represents the movement of the chassis.
struct Movement 
{
    MovementAxis xAxis;
    MovementAxis yAxis;
    MovementAxis wAxis;

    void set_all_speeds(int x, int y, int w);
};
