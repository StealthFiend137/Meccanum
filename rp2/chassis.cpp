#include <pico/stdlib.h>

class Chassis
{
private:

    uint _command_timeout_ms;

    struct Axis
    {
        int Speed;
        int StartTime;
    };

    struct Movement movement 
    {
        Axis xAxis;
        Axis yAxis;
        Axis wAxis;
    };

    void set_axis(Axis axis, int speed)
    {
        axis.Speed = speed;
        axis.StartTime = to_ms_since_boot(get_absolute_time());
    };

    void set_axis(Axis axis, int speed, int start_time)
    {
        axis.Speed = speed;
        axis.StartTime = start_time;
    }

public:

    Chassis(uint comamnd_timeout_ms) : _command_timeout_ms(comamnd_timeout_ms) {};

    set_x_axis(int speed)
    {
        set_axis(movement.xAxis, speed)
    };

    set_y_axis(int speed)
    {
        set_axis(movement.yAxis, speed)
    };

    set_w_axis(int speed)
    {
        set_axis(movement.wAxis, speed)
    };

    set_all_axis(int x, int y, int z)
    {
        int start_time = to_ms_since_boot(get_absolute_time();
        set_axis(movement.xAxis, x, start_time);
        set_axis(movement.yAxis, y, start_time);
        set_axis(movement.zAxis, z, start_time);
    }
};
