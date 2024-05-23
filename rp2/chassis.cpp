#include <pico/stdlib.h>

class Chassis
{
private:

    uint _command_timeout_ms;

    struct Axis
    {
        int Speed;
        int StartTime;
        bool ChangedSinceLastRead;
    };

    struct Movement movement 
    {
        Axis xAxis;
        Axis yAxis;
        Axis wAxis;
    };

    void set_axis(Axis axis, int speed, int start_time)
    {
        axis.Speed = speed;
        axis.StartTime = start_time;
        axis.ChangedSinceLastRead = true;
    };

    void set_axis(Axis axis, int speed)
    {
        int start_time = to_ms_since_boot(get_absolute_time());
        set_axis(axis, speed, start_time);
    };

    int get_axis(Axis axis)
    {
        axis.ChangedSinceLastRead = false;
        return axis.speed;
    }

public:

    Chassis(uint comamnd_timeout_ms) : _command_timeout_ms(comamnd_timeout_ms) {};

    set_x_axis(int speed)
    {
        set_axis(movement.xAxis, speed)
    };

    bool x_axis_changed()
    {
        return movement.xAxis.ChangedSinceLastRead;
    }

    int get_x_axis()
    {
        return get_axis(movement.xAxis);
    }

    set_y_axis(int speed)
    {
        set_axis(movement.yAxis, speed)
    };

    bool y_axis_changed()
    {
        return movement.yAxis.ChangedSinceLastRead;
    }

    int get_y_axis()
    {
        return get_axis(movement.yAxis);
    }

    set_w_axis(int speed)
    {
        set_axis(movement.wAxis, speed)
    };

    bool w_axis_changed()
    {
        return movement.wAxis.ChangedSinceLastRead;
    }

    int get_w_axis()
    {
        return get_axis(movement.wAxis);
    }

    set_all_axis(int x, int y, int z)
    {
        int start_time = to_ms_since_boot(get_absolute_time();
        set_axis(movement.xAxis, x, start_time);
        set_axis(movement.yAxis, y, start_time);
        set_axis(movement.zAxis, z, start_time);
    }
};
