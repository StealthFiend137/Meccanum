#pragma once

class I2cMultiplexer;

class I2cMultiplexedChannel
{
private:
    int _clock_pin;
    I2cMultiplexer* _multiplexer;

public:
    I2cMultiplexedChannel(I2cMultiplexer* multiplexer, int clock_pin);
    void i2c_write_blocking();
    void i2c_read_blocking();
};
