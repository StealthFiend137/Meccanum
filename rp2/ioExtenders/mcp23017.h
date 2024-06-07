#pragma once
#include <hardware/i2c.h>
#include "../i2cMultiplexer/i2cMultiplexer.h"

namespace IoExtenders
{
    class Mcp23017;
};

class IoExtenders::Mcp23017
{
private:

    int _i2c_address;
    I2cMultiplexedChannel* _i2c_multiplexed_channel;

public:

    enum class Bank
    {
        A,
        B,
    };

    Mcp23017(I2cMultiplexedChannel* i2c_multiplexed_channel, int i2c_address);
    void SetPinState(Bank bank, int pinNumber, bool highLow);
};
