#pragma once
#include <vector>
#include "movementAxis.h"

/// @brief Control and monitoring of the meccanum chassis.
class Chassis
{
public:

    /// @brief Create a new instance of the Chassis class.
    /// @param comamnd_timeout_ms The amount of time in milliseconds until a command is cancelled. The time is reset every time it is refreshed.
    Chassis(uint comamnd_timeout_ms);

    /// @brief Sets all of the movement axes simultaneously.
    /// @param xVelocity The value of the x axis.
    /// @param yVelocity The value of the y axis.
    /// @param wVelocity The value of the w axis.
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

    void register_callback(void(*callback)());

    enum ModificationFlags
    {
        xAxisModified = 1 >> 0,
        yAxisModified = 1 >> 1,
        wAxisModified = 1 >> 2
    };

private:

    /// @brief The x axis.
    MovementAxis xAxis;

    /// @brief The y axis.
    MovementAxis yAxis;

    /// @brief The w axis.
    MovementAxis wAxis;

    /// @brief Vector of callbacks used to notify of changes to chassis values.
    std::vector<void (*)()> _modificationCallbacks;

    /// @brief Backing field for the amount of time in milliseconds without renewall until an action is stopped.
    uint _command_timeout_ms;

    /// @brief Private method to set the speed of a provided axis.
    /// @param movementAxis The axis to set.
    /// @param speed The speed to set the axis to.
    void set_axis(MovementAxis* movementAxis, int speed);

    /// @brief Private method to set the speed of a provided axis at a specified time.
    /// @param movementAxis The axis to set.
    /// @param speed The speed to set the axis to.
    /// @param update_time The time to record as when this change was made.
    void set_axis(MovementAxis* movementAxis, int speed, int start_time);

    /// @brief Private method to get the speed of a specified axis.
    /// @param MovementAxis The axis to get the speed of.
    /// @return Returns the speed of the provided axis.
    int get_axis(MovementAxis* MovementAxis);
};
