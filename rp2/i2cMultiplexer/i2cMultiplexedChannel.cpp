#include "i2cMultiplexer.h"

I2cMultiplexedChannel::I2cMultiplexedChannel(I2cMultiplexer* multiplexer, int clock_pin) :
    _multiplexer(multiplexer), _clock_pin(clock_pin)
{

};

void I2cMultiplexedChannel::i2c_read_blocking()
{
    this->_multiplexer->switch_channel(_clock_pin);
};

void I2cMultiplexedChannel::i2c_write_blocking()
{
    this->_multiplexer->switch_channel(_clock_pin);
};