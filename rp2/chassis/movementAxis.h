#pragma once

struct MovementAxis
{
private:

    int _velocity = 0;
    int StartTime;
    bool ChangedSinceLastRead;

public:

    void set_speed(int speed);
    void set_speed(int speed, int start_time);
    int get_speed(void);
};
