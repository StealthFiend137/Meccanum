#pragma once

struct AxisState
{
private:

    int default_speed;
    int decay_time_ms;

    int speed = 0;
    bool isDecaying = false;
    int decayStartTimestamp = 0;

public:

    AxisState(int default_speed, int decay_time_ms);
    bool has_decayed(int current_time);
    void set_speed(int speed, int update_time);
    int get_speed();
};