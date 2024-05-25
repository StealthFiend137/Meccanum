#include "chassis.h"
#include <pico/stdlib.h>

/// Public

Chassis::Chassis(uint comamnd_timeout_ms) : _command_timeout_ms(comamnd_timeout_ms)
{
};

void Chassis::set_all_axis(int xVelocity, int yVelocity, int wVelocity)
{
    int start_time = to_ms_since_boot(get_absolute_time());
    set_axis(&_movement.xAxis, xVelocity, start_time);
    set_axis(&_movement.yAxis, yVelocity, start_time);
    set_axis(&_movement.wAxis, wVelocity, start_time);
};

void Chassis::set_x_axis(int velocity)
{
    set_axis(&_movement.xAxis, velocity);
};

int Chassis::get_x_axis()
{
    return get_axis(&_movement.xAxis);
};

/// Private

void Chassis::set_axis(MovementAxis* movementAxis, int speed)
{
    movementAxis->set_speed(speed);
};

void Chassis::set_axis(MovementAxis* movementAxis, int speed, int start_time)
{
    movementAxis->set_speed(speed, start_time);
};

int Chassis::get_axis(MovementAxis* MovementAxis)
{
    return MovementAxis->get_speed();
};
