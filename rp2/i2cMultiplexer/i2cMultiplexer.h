#pragma once
#include <hardware/i2c.h>
#include "i2cMultiplexedChannel.h"

class I2cMultiplexer
{
private:
    i2c_inst_t* _i2c_instance;
    int _data_pin;
    int _current_clock_pin = -1;
    
public:
    I2cMultiplexer(i2c_inst_t* i2c_instance, int data_pin);
    I2cMultiplexedChannel* create_channel(int clock_pin);
    void switch_channel(int clock_pin);
};
