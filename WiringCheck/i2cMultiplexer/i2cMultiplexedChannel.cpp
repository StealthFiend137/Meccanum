#include "i2cMultiplexer.h"

#include <cstdio>

I2cMultiplexedChannel::I2cMultiplexedChannel(I2cMultiplexer* multiplexer, int clock_pin) :
    _multiplexer(multiplexer), _clock_pin(clock_pin)
{
};

int I2cMultiplexedChannel::i2c_read_blocking(uint8_t addr, uint8_t *dst, size_t len, bool nostop)
{
    return this->_multiplexer->i2c_read_blocking(this->_clock_pin, addr, dst, len, nostop);
};

int I2cMultiplexedChannel::i2c_write_blocking(uint8_t addr, const uint8_t *src, size_t len, bool nostop)
{
    return this->_multiplexer->i2c_write_blocking(_clock_pin, addr, src, len, nostop);
};