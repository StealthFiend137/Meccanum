#include <pico/stdlib.h>

#include "mcp23017.h"
#include "mcp23017_ControlPin.h"

/// @brief Provides control of the output pins of an MCP23017.
/// @param i2c_instance The already connected i2c instance.
/// @param i2c_address The address of the MCP23017.
/// @param enable_pin The Rp2040 pin tied to the reset input.
IoExtenders::Mcp23017::Mcp23017(I2cMultiplexedChannel* i2c_multiplexed_channel, int i2c_address, int enable_pin):
    _i2c_multiplexed_channel(i2c_multiplexed_channel), _i2c_address(i2c_address), _enable_pin(enable_pin)
{
    this->reset();
};

void IoExtenders::Mcp23017::reset()
{
    gpio_init(_enable_pin);
    gpio_set_dir(_enable_pin, GPIO_OUT);
    gpio_put(_enable_pin, 1);
}

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
    bank = (int)pin > 10 ? Bank::A : Bank::B;
    pinNumber = ((int)pin % 20) - 1;
}

void IoExtenders::Mcp23017::set_pin_as_output(const IoPin pin)
{
    Bank bank;
    int pinNumber;
    this->get_bank_and_number(pin, bank, pinNumber);    
    this->set_pin_as_output(bank, pinNumber);
    this->set_pin_state(bank, pinNumber, false);
}

void IoExtenders::Mcp23017::set_pin_state(const IoPin pin, const ControlPins::DigitalControlPin::PinState  state)
{
    Bank bank;
    int pinNumber;
    this->get_bank_and_number(pin, bank, pinNumber);
    this->set_pin_state(bank, pinNumber, (bool)state);
}

ControlPins::DigitalControlPin* IoExtenders::Mcp23017::get_DigitalControlPin(IoPin pin)
{
    Bank bank;
    int pinNumber;
    this->get_bank_and_number(pin, bank, pinNumber);
    return new IoExtenders::Mcp23017_ControlPin(this, bank, pinNumber);
}

ControlPins::DigitalControlPin* IoExtenders::Mcp23017::get_DigitalOutputControlPin(IoPin pin, ControlPins::DigitalControlPin::PinState  initialState)
{
    Bank bank;
    int pinNumber;
    this->get_bank_and_number(pin, bank, pinNumber);
    
    ControlPins::DigitalControlPin* controlPin = new IoExtenders::Mcp23017_ControlPin(this, bank, pinNumber);
    this->set_pin_as_output(pin);
    this->set_pin_state(pin, initialState);

    return controlPin;
}