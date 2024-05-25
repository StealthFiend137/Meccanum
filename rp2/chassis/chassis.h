#pragma once
#include "movementAxis.h"

class Chassis
{
public:

    /// @brief Create a new instance of the Chassis class.
    /// @param comamnd_timeout_ms The amount of time in milliseconds until a command is cancelled. The time is reset every time it is refreshed.
    Chassis(uint comamnd_timeout_ms);

    void set_all_axis(int xVelocity, int yVelocity, int wVelocity);

    /// @brief Sets the speed of the axis.
    /// @param speed The requested speed of the axis.
    void set_x_axis(int speed);

    /// @brief Gets the speed of the axis.
    /// @return Returns the speed of the axis. 
    int get_x_axis();

    /// @brief Sets the speed of the axis.
    /// @param speed The requested speed of the axis.
    void set_y_axis(int speed);

    /// @brief Gets the speed of the axis.
    /// @return Returns the speed of the axis. 
    int get_y_axis();

    /// @brief Sets the speed of the axis.
    /// @param speed The requested speed of the axis.
    void set_w_axis(int speed);

    /// @brief Gets the speed of the axis.
    /// @return Returns the speed of the axis. 
    int get_w_axis();

private:

    MovementAxis xAxis;
    MovementAxis yAxis;
    MovementAxis wAxis;

    /// @brief Backing field for the amount of time in milliseconds without renewall until an action is stopped.
    uint _command_timeout_ms;

    void set_axis(MovementAxis* movementAxis, int speed);
    void set_axis(MovementAxis* movementAxis, int speed, int start_time);
    int get_axis(MovementAxis* MovementAxis);
};
