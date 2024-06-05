#include "i2cMultiplexedChannel.h"

I2cMultiplexedChannel::I2cMultiplexedChannel(I2cMultiplexer* multiplexer) : _multiplexer(multiplexer)
{

}


void I2cMultiplexedChannel::i2c_read_blocking()
{
    this->_multiplexer->switch_channel(_clock_pin);
};
