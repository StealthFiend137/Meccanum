#include "chassis.h"
#include <pico/stdlib.h>
#include <vector>

/// @brief Creates a new instance of the Chassis class.
/// @param comamnd_timeout_ms The timeout in ms before items return to their default state.
Chassis::Chassis(uint comamnd_timeout_ms) : _command_timeout_ms(comamnd_timeout_ms)
{
};

/// @brief Sets all of the movement axes simultaneously.
/// @param xVelocity The value of the x axis.
/// @param yVelocity The value of the y axis.
/// @param wVelocity The value of the w axis.
void Chassis::set_all_axis(int xVelocity, int yVelocity, int wVelocity)
{
    int modified_time = to_ms_since_boot(get_absolute_time());
    set_axis(xVelocity, &xAxis, xAxisModified, modified_time);
    set_axis(yVelocity, &yAxis, yAxisModified, modified_time);
    set_axis(wVelocity, &wAxis, wAxisModified, modified_time);
};

/// @brief Sets the value of the x axis.
/// @param velocity The value of the x axis.
void Chassis::set_x_axis(int velocity)
{
    Modified modifiedAxis = xAxisModified;
    MovementAxis* axis = &xAxis;
    set_axis(velocity, axis, modifiedAxis);
};

/// @brief Gets the value of the x axis.
/// @return Returns the value of the x axis.
int Chassis::get_x_axis()
{
    return get_axis(&xAxis);
};

/// @brief Sets the value of the y axis.
/// @param velocity The value of the y axis.
void Chassis::set_y_axis(int velocity)
{
    Modified modifiedAxis = yAxisModified;
    MovementAxis* axis = &yAxis;
    set_axis(velocity, axis, modifiedAxis);
};

/// @brief Gets the value of the y axis.
/// @return Returns the value of the y axis.
int Chassis::get_y_axis()
{
    return get_axis(&yAxis);
};

/// @brief Sets the value of the w axis.
/// @param velocity The value of the w axis.
void Chassis::set_w_axis(int velocity)
{
    Modified modifiedAxis = wAxisModified;
    MovementAxis* axis = &wAxis;
    set_axis(velocity, axis, modifiedAxis);
};

/// @brief Gets the value of the w axis.c++ 
/// @return Returns the value of the w axis.
int Chassis::get_w_axis()
{
    return get_axis(&wAxis);
};

void Chassis::register_callback(Modified (*callback)())
{
    this->_modificationCallbacks.push_back(callback);
};

/// @brief Private method to set the speed of a provided axis.
/// @param speed The speed to set the axis to.
/// @param movementAxis The axis to set.
/// @param modifiedAxis The bitwise value of the modified axis.
void Chassis::set_axis(int speed, MovementAxis* movementAxis, Modified modifiedAxis)
{
    movementAxis->set_speed(speed);
};

/// @brief Private method to set the speed of a provided axis at a specified time.
/// @param movementAxis The axis to set.
/// @param speed The speed to set the axis to.
/// @param modifiedAxis The bitwise value of the modified axis.
/// @param update_time The time to record as when this change was made.
void Chassis::set_axis(int speed, MovementAxis* movementAxis, Modified modifiedAxis, int update_time)
{
    movementAxis->set_speed(speed, update_time);
};

/// @brief Private method to get the speed of a specified axis.
/// @param MovementAxis The axis to get the speed of.
/// @return Returns the speed of the provided axis.
int Chassis::get_axis(MovementAxis* MovementAxis)
{
    return MovementAxis->get_speed();
};
