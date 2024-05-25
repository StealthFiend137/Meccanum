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
    int start_time = to_ms_since_boot(get_absolute_time());
    set_axis(&xAxis, xVelocity, start_time);
    set_axis(&yAxis, yVelocity, start_time);
    set_axis(&wAxis, wVelocity, start_time);
};

/// @brief Sets the value of the x axis.
/// @param velocity The value of the x axis.
void Chassis::set_x_axis(int velocity)
{
    set_axis(&xAxis, velocity);
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
    set_axis(&yAxis, velocity);
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
    set_axis(&wAxis, velocity);
};

/// @brief Gets the value of the w axis.c++ 
/// @return Returns the value of the w axis.
int Chassis::get_w_axis()
{
    return get_axis(&wAxis);
};

void Chassis::register_callback(void(*callback)())
{
    this->_modificationCallbacks.push_back(callback);
};

/// @brief Private method to set the speed of a provided axis.
/// @param movementAxis The axis to set.
/// @param speed The speed to set the axis to.
void Chassis::set_axis(MovementAxis* movementAxis, int speed)
{
    movementAxis->set_speed(speed);
    // ModificationFlags flags = static_cast<ModificationFlags>(
    //     xAxisModified | yAxisModified | wAxisModified
    // );
};

/// @brief Private method to set the speed of a provided axis at a specified time.
/// @param movementAxis The axis to set.
/// @param speed The speed to set the axis to.
/// @param update_time The time to record as when this change was made.
void Chassis::set_axis(MovementAxis* movementAxis, int speed, int update_time)
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
