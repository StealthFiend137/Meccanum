#pragma once
#include "motor.h"

namespace Motors
{
    class OpenLoop;  
};

class Motors::OpenLoop : public virtual Motors::Motor
{
public:

    enum class Orientation
    {
        clockwise,
        anticlockwise
    };

    int _pwm_gpio;
    int _direction_gpio;
    Orientation _orientation;

    OpenLoop(int pwm_gpio, int direction_gpio, Orientation orientation);
    void set_speed(int speed_in_percent) override;
};





