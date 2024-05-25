#pragma once
#include "chassis.h"
#include <pico/stdlib.h>

Chassis::Chassis(uint comamnd_timeout_ms) : _command_timeout_ms(comamnd_timeout_ms) {};

/*
/// @brief Controller for all aspects of the connected chassis.  Such as movement, sound, battery monitoring etc.
class Chassis
{
private:

    /// @brief Backing field for the amount of time in milliseconds without renewall until an action is stopped.
    uint _command_timeout_ms;

    
    struct MovementAxis
    {
    private:

        int _speed;
        int StartTime;
        bool ChangedSinceLastRead;

    public:

        void set_speed(int speed, int start_time)
        {
            _speed = speed;
            StartTime = start_time;
        }

        void set_speed(int speed)
        {
            int start_time = to_ms_since_boot(get_absolute_time());
            set_speed(speed, start_time);
        }

        int get_speed(void)
        {
            ChangedSinceLastRead = false;
            return _speed;
        }
    };

    struct Movement 
    {
        MovementAxis xAxis;
        MovementAxis yAxis;
        MovementAxis wAxis;

        void set_all_speeds(int x, int y, int w)
        {
            int start_time = to_ms_since_boot(get_absolute_time());
            xAxis.set_speed(x, start_time);
            yAxis.set_speed(y, start_time);
            wAxis.set_speed(w, start_time);
        }
    };

public:

    Movement movement;

    /// @brief Create a new instance of the Chassis class.
    /// @param comamnd_timeout_ms The amount of time in milliseconds until a command is cancelled. The time is reset every time it is refreshed.
    Chassis(uint comamnd_timeout_ms) : _command_timeout_ms(comamnd_timeout_ms) {};
};
*/