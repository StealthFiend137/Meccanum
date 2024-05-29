#include <pico/stdlib.h>
#include <algorithm>
#include <cstdio>

#include "chassis.h"
#include "axisState.h"

struct AxesStatus
{
    AxisState xAxis{0, 5000};
    AxisState yAxis{0, 5000};
    AxisState wAxis{0, 5000};
};

/// @brief Creates a new instance of the Chassis class.
/// @param comamnd_timeout_ms The timeout in ms before items return to their default state.
Chassis::Chassis(const uint comamnd_timeout_ms)
{
    this->_command_timeout_ms = comamnd_timeout_ms;

    const int interval_ms = 2000;
    add_repeating_timer_ms(interval_ms, decay_callback, NULL, &decay_timer);
};

bool Chassis::decay_callback(struct repeating_timer *t)
{
    int ms_since_boot = to_ms_since_boot(get_absolute_time());
    printf("decay timer.%d\n", ms_since_boot);
    //get_changed_axes(ms_since_boot);

    return true;
};

void Chassis::get_changed_axes(int current_time)
{
    Modified newModified;

    // if(_axes_status.xAxis.has_decayed(current_time)) newModified = static_cast<Modified>(newModified | xAxisModified);
    // if(_axes_status.yAxis.has_decayed(current_time)) newModified = static_cast<Modified>(newModified | yAxisModified);
    // if(_axes_status.wAxis.has_decayed(current_time)) newModified = static_cast<Modified>(newModified | wAxisModified);

    if(Modified::none == newModified) return;
};

/// @brief Sets all of the movement axes simultaneously.
/// @param xVelocity The value of the x axis.
/// @param yVelocity The value of the y axis.
/// @param wVelocity The value of the w axis.
void Chassis::set_all_axes(int xVelocity, int yVelocity, int wVelocity)
{
    int modified_time = to_ms_since_boot(get_absolute_time());
    set_axis(xVelocity, &xAxis, Modified::xAxisModified, modified_time);
    set_axis(yVelocity, &yAxis, Modified::yAxisModified, modified_time);
    set_axis(wVelocity, &wAxis, Modified::wAxisModified, modified_time);

    Modified modified = Modified::wAxisModified | Modified::xAxisModified | Modified::yAxisModified;
    notify_change(modified);
};

void Chassis::notify_change(Modified modified)
{
    std::for_each(this->_modificationCallbacks.begin(), this->_modificationCallbacks.end(), [modified](void (*callback)(Modified)) {
        callback(modified);
    });
};

/// @brief Sets the value of the w axis.
/// @param velocity The value of the w axis.
void Chassis::set_w_axis(int velocity)
{
    Modified modifiedAxis = Modified::wAxisModified;
    AxisState* axis = &wAxis;
    set_axis(velocity, axis, modifiedAxis);
    notify_change(Modified::wAxisModified);
};

/// @brief Gets the value of the w axis.c++ 
/// @return Returns the value of the w axis.
int Chassis::get_w_axis()
{
    return get_axis(&wAxis);
};

/// @brief Sets the value of the x axis.
/// @param velocity The value of the x axis.
void Chassis::set_x_axis(int velocity)
{
    Modified modifiedAxis = Modified::xAxisModified;
    AxisState* axis = &xAxis;
    set_axis(velocity, axis, modifiedAxis);
    notify_change(Modified::xAxisModified);
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
    Modified modifiedAxis = Modified::yAxisModified;
    AxisState* axis = &yAxis;
    set_axis(velocity, axis, modifiedAxis);
    notify_change(Modified::yAxisModified);
};

/// @brief Gets the value of the y axis.
/// @return Returns the value of the y axis.
int Chassis::get_y_axis()
{
    return get_axis(&yAxis);
};

void Chassis::register_callback(void (*callback)(Modified))
{
    this->_modificationCallbacks.push_back(callback);
};

/// @brief Private method to set the speed of a provided axis.
/// @param speed The speed to set the axis to.
/// @param movementAxis The axis to set.
/// @param modifiedAxis The bitwise value of the modified axis.
void Chassis::set_axis(int speed, AxisState* movementAxis, Modified modifiedAxis)
{
    int update_time = to_ms_since_boot(get_absolute_time());
    set_axis(speed, movementAxis, modifiedAxis, update_time);
};

/// @brief Private method to set the speed of a provided axis at a specified time.
/// @param movementAxis The axis to set.
/// @param speed The speed to set the axis to.
/// @param modifiedAxis The bitwise value of the modified axis.
/// @param update_time The time to record as when this change was made.
void Chassis::set_axis(int speed, AxisState* movementAxis, Modified modifiedAxis, int update_time)
{
    movementAxis->set_speed(speed, update_time);
};

/// @brief Private method to get the speed of a specified axis.
/// @param MovementAxis The axis to get the speed of.
/// @return Returns the speed of the provided axis.
int Chassis::get_axis(AxisState* MovementAxis)
{
    return MovementAxis->get_speed();
};
