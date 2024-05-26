#include "AxisState.h"

AxisState::AxisState(int default_speed, int decay_time_ms)
{
    this->default_speed = default_speed;
    this->decay_time_ms = decay_time_ms;
};

bool AxisState::has_decayed(int current_time)
{
    if(!this->isDecaying) return false;
    if(this->decayStartTimestamp+decay_time_ms > current_time) return false;

    this->isDecaying = false;
    this->speed = this->default_speed;
    return true;
};

void AxisState::set_speed(int speed, int update_time)
{
    this->isDecaying = true;
    this->decayStartTimestamp = update_time;
    this->speed = speed;
};

int AxisState::get_speed()
{
    return this->speed;
};
