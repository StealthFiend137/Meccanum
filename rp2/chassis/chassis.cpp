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

/// @brief Sets the value of the x axis.
/// @param velocity The value of the x axis.
void Chassis::set_x_axis(int velocity)
{
    set_axis(&_movement.xAxis, velocity);
};

/// @brief Gets the value of the x axis.
/// @return Returns the value of the x axis.
int Chassis::get_x_axis()
{
    return get_axis(&_movement.xAxis);
};

/// @brief Sets the value of the y axis.
/// @param velocity The value of the y axis.
void Chassis::set_y_axis(int velocity)
{
    set_axis(&_movement.yAxis, velocity);
};

/// @brief Gets the value of the y axis.
/// @return Returns the value of the y axis.
int Chassis::get_y_axis()
{
    return get_axis(&_movement.yAxis);
};

/// @brief Sets the value of the w axis.
/// @param velocity The value of the w axis.
void Chassis::set_w_axis(int velocity)
{
    set_axis(&_movement.wAxis, velocity);
};

/// @brief Gets the value of the w axis.
/// @return Returns the value of the w axis.
int Chassis::get_w_axis()
{
    return get_axis(&_movement.wAxis);
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
