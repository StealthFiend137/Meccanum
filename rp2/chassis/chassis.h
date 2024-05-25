#pragma once

class Chassis
{
public:

    /// @brief Create a new instance of the Chassis class.
    /// @param comamnd_timeout_ms The amount of time in milliseconds until a command is cancelled. The time is reset every time it is refreshed.
    Chassis(uint comamnd_timeout_ms);

    /// @brief Sets the speed of the axis.
    /// @param speed The requested speed of the axis.
    void set_x_axis(int speed);

    /// @brief Gets the speed of the axis.
    /// @return Returns the speed of the axis. 
    int get_x_axis();

private:

    /// @brief Backing field for the amount of time in milliseconds without renewall until an action is stopped.
    uint _command_timeout_ms;
};
