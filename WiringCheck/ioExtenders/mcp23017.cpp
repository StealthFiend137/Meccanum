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
    uint8_t pinMask = 1 << (pinNumber);
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
    uint8_t pinMask = 1 << (pinNumber);
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

void IoExtenders::Mcp23017::get_bank_and_number(const IoPin pin, Bank& bank, int& pinNumber)
{
    switch(pin)
    {
        case IoPin::GPA0:
        case IoPin::GPA1:
        case IoPin::GPA2:
        case IoPin::GPA3:
        case IoPin::GPA4:
        case IoPin::GPA5:
        case IoPin::GPA6:
        case IoPin::GPA7:
            bank = Bank::A;
            pinNumber = (int)pin-21;
            break;
        
        case IoPin::GPB0:
        case IoPin::GPB1:
        case IoPin::GPB2:
        case IoPin::GPB3:
        case IoPin::GPB4:
        case IoPin::GPB5:
        case IoPin::GPB6:
        case IoPin::GPB7:
            bank = Bank::B;
            pinNumber = (int)pin-1;
            break;
    }
}

void IoExtenders::Mcp23017::set_pin_as_output(const IoPin pin)
{
    Bank bank;
    int pinNumber;
    this->get_bank_and_number(pin, bank, pinNumber);    
    this->set_pin_as_output(bank, pinNumber);
    this->set_pin_state(bank, pinNumber, false);
}

void IoExtenders::Mcp23017::set_pin_state(const IoPin pin, const IoPinOutputState state)
{
    Bank bank;
    int pinNumber;
    this->get_bank_and_number(pin, bank, pinNumber);
    this->set_pin_state(bank, pinNumber, (bool)state);
}
