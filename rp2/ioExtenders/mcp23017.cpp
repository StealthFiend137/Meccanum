#include "mcp23017.h"
#include <cstdio>

/// @brief Provides control of the output pins of an MCP23017.
/// @param i2c_instance The already connected i2c instance.
/// @param i2c_address The address of the MCP23017.
IoExtenders::Mcp23017::Mcp23017(I2cMultiplexedChannel* i2c_multiplexed_channel, int i2c_address):
    _i2c_multiplexed_channel(i2c_multiplexed_channel), _i2c_address(i2c_address)
{
};

void IoExtenders::Mcp23017::SetPinState(Bank bank, int pinNumber, bool highLow)
{
    // Get the current output register for the specified bank.
    // Bitmask the pin against the new pinstate.
    // Write the new pinstate (unless it's already correct)

    uint8_t buffer[2];
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    this->_i2c_multiplexed_channel->i2c_write_blocking(_i2c_address, buffer, 2, false);

    buffer[0] = 0x12;
    buffer[1] = highLow ? 0x00 : 0xff;
    this->_i2c_multiplexed_channel->i2c_write_blocking(_i2c_address, buffer, 2, false);
    printf("Register state %u\n", buffer[0]);
};
