#pragma once

namespace Motor
{
    class Motor;
};

class Motor::Motor
{
public:

    virtual void set_speed(int speed_in_percent) = 0;
};

