#pragma once
#include <hardware/i2c.h>
#include "i2cMultiplexedChannel.h"

class I2cMultiplexer
{
private:
    i2c_inst_t* _i2c_instance;
    int _data_pin;
    int _enable_pin;
    int _current_clock_pin = -1;
    void switch_clock_pin(int clock_pin);
    
public:
    I2cMultiplexer(i2c_inst_t* i2c_instance, int data_pin, int enable_pin);
    I2cMultiplexedChannel* create_channel(int clock_pin);
    int i2c_write_blocking(int clock_pin, uint8_t addr, const uint8_t *src, size_t len, bool nostop);
    int i2c_read_blocking(int clock_pin, uint8_t addr, uint8_t *dst, size_t len, bool nostop);
};
