#include "mcp23017.h"

/// @brief Provides control of the output pins of an MCP23017.
/// @param i2c_instance The already connected i2c instance.
/// @param i2c_address The address of the MCP23017.
IoExtenders::Mcp23017::Mcp23017(I2cMultiplexedChannel* i2c_multiplexed_channel, int i2c_address):
    _i2c_multiplexed_channel(i2c_multiplexed_channel), _i2c_address(i2c_address)
{
};

void IoExtenders::Mcp23017::set_pin_as_output(Bank bank, int pinNumber)
{
    uint8_t registerOffset = (Bank::A == bank) ? PORTA : PORTB;
    uint8_t address = (uint8_t)IODIR + registerOffset;
    uint8_t read_buffer[1];

    this->_i2c_multiplexed_channel->i2c_write_blocking(_i2c_address, &address, 1, KEEP_CONTROL);
    this->_i2c_multiplexed_channel->i2c_read_blocking(_i2c_address, read_buffer, 1, FINISHED_WITH_BUS);

    uint8_t current = read_buffer[0];
    uint8_t pinMask = 1 << (pinNumber-1);
    uint8_t write_buffer[2] { address };
    write_buffer[1] = (current & static_cast<uint8_t>(~pinMask));
    this->_i2c_multiplexed_channel->i2c_write_blocking(_i2c_address, write_buffer, 2, FINISHED_WITH_BUS);
};

void IoExtenders::Mcp23017::set_pin_state(Bank bank, int pinNumber, bool highLow)
{
    uint8_t registerOffset = (Bank::A == bank) ? PORTA : PORTB;
    uint8_t address = OLAT + registerOffset;
    uint8_t read_buffer[1];

    this->_i2c_multiplexed_channel->i2c_write_blocking(_i2c_address, &address, 1, KEEP_CONTROL);
    this->_i2c_multiplexed_channel->i2c_read_blocking(_i2c_address, read_buffer, 1, FINISHED_WITH_BUS);

    uint8_t write_buffer[2] { address };
    uint8_t pinMask = 1 << (pinNumber-1);
    if (highLow)
    {
        write_buffer[1] = (read_buffer[0] | pinMask);
    }
    else
    {
        write_buffer[1] = (read_buffer[0] & ~pinMask);
    }

    this->_i2c_multiplexed_channel->i2c_write_blocking(_i2c_address, write_buffer, 2, FINISHED_WITH_BUS);
};
