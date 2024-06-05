#include "i2cMultiplexer.h"
#include "i2cMultiplexedChannel.h"

I2cMultiplexer::I2cMultiplexer(i2c_inst_t* i2c_instance, int data_pin):
    _i2c_instance(i2c_instance), _data_pin(data_pin)
{
};

void I2cMultiplexer::switch_channel(int clock_pin)
{
    // release the this->_current_clock_pin pin
    // set clock_pin to be the i2c instance's clock pin.l
    this->_current_clock_pin = clock_pin;
};

I2cMultiplexedChannel* I2cMultiplexer::create_channel(int clock_pin)
{
    return new I2cMultiplexedChannel(this, clock_pin);
}