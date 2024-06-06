#pragma once

class I2cMultiplexer;

class I2cMultiplexedChannel
{
private:
    int _clock_pin;
    I2cMultiplexer* _multiplexer;

public:
    I2cMultiplexedChannel(I2cMultiplexer* multiplexer, int clock_pin);
    int i2c_read_blocking(uint8_t addr, uint8_t *dst, size_t len, bool nostop);
    int i2c_write_blocking(uint8_t addr, const uint8_t *src, size_t len, bool nostop);
};
