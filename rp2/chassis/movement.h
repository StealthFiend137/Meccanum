#pragma once
#include "movementAxis.h"

struct Movement 
{
    MovementAxis xAxis;
    MovementAxis yAxis;
    MovementAxis wAxis;

    void set_all_speeds(int x, int y, int w);
};
