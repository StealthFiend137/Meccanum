#pragma once

struct MovementAxis
{
private:

    int _speed;
    int StartTime;
    bool ChangedSinceLastRead;

public:

    void set_speed(int speed, int start_time);
    void set_speed(int speed);
    int get_speed(void);
};
