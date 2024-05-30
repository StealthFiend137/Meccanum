#include "openLoop.h"
#include <cstdio>

Motors::OpenLoop::OpenLoop(int pwm_gpio, int direction_gpio, Orientation orientation):
    _pwm_gpio(pwm_gpio), _direction_gpio(direction_gpio), _orientation(orientation)
{
};

void Motors::OpenLoop::set_speed(int speed_in_percent)
{
    printf("Setting speed to %d\n", speed_in_percent);
};