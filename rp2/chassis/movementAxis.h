#pragma once
#include "velocity.h"

struct MovementAxis
{
private:

    Velocity _velocity;
    int StartTime;
    bool ChangedSinceLastRead;

public:

    void set_speed(int speed, int start_time);
    void set_speed(int speed);
    int get_speed(void);
};
