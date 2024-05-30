#pragma once

namespace Motors
{
    class Motor;
};

class Motors::Motor
{
public:

    virtual void set_speed(int speed_in_percent) = 0;
};

