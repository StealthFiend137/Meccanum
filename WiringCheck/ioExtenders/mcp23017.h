#pragma once

#define KEEP_CONTROL true
#define FINISHED_WITH_BUS false

#include <hardware/i2c.h>
#include "../i2cMultiplexer/i2cMultiplexer.h"

namespace IoExtenders
{
    class Mcp23017;
};

#define PORTA   0x00
#define PORTB   0x01

// IOCON.BANK seems to be set to 0 at startup.

#define IODIR   0X00   // IO Direction. 1 for input, 0 for output.
#define IPOL    0x02   // Input polarity. 1 to invert register, 0 to match the pin.
#define GPINTEN 0x04   // Interrupt-on-change. 1 to enable interrupt on the pin, 0 to disable.
#define DEFVAL  0x06   // Default compare register for interrupt-on-change. If INTCON is 1, then a change the opposite corrosponding pin will cause an interrupt
#define INTCON  0x08   // Interrupt control.  1 to compare against the corrosponding DEFVAL bit, 0 to compare against previous value.
#define IOCON   0x0A   // Configuration Register.
                       // Name    bit  Purpose
                       // BANK    7    Configure how the registers are mapped.  Defaults to BANK=0. Changes take effect when write is complete.
                       // MIRROR  6    Interrupt Mirror. 1 to OR pins between the ports so that a signal on either pin will cause both interrupts. 0 to for each pin to cause an interrupt for it's own port only.
                       // SEQOP   5    Sequental Operation.
                       // DISSLW  4    Slew Rate. When enabled the slew rate on the SDA pin will be controlled when driving high to low.
                       // HAEN    3    Hardware Address Enable.  Enable hardware addressing. (for MCP23S17 ONLY).
                       // ODR     2    Open Drain.  Configures INT pin as open-drain output. 1 for Open drain output (INTPOL bit ignored), 0 for active driver output (INTPOL sets the polarity);
                       // INTPOL  1    Sets the polarity of the INT output pin. 1 for active-high, 0 for active-low.
                       // -       0    No function. Always reads as 0.
#define GPPU    0x0C   // Pull-up resister configuration. 1 Enables the pull-up resister, 0 disables it.
#define INTF    0x0E   // Interrupt Flag. Indicates which pin caused the interrupt. 1 when the pin caused the interrupt, 0 when interrupt not pending.
#define INTCAP  0x10   // Interrupt Captured.  Captures the GPIO port value at the time the interrupt occured.  READ ONLY. Remains unchanged until GPIO or INTCAP read.
#define GPIO    0x12   // Data Ports (read the value on the port (pin) input).  READ ONLY, but writing to this register actually updates the OLAT regsiter.
#define OLAT    0x14   // Output Latch (write to change the port (pin) output)

class IoExtenders::Mcp23017
{
public:
    enum class Bank;
    enum class IoPin : int;
    enum class IoPinOutputState : bool;

private:

    int _i2c_address;
    I2cMultiplexedChannel* _i2c_multiplexed_channel;

    void get_bank_and_number(const IoPin pin, Bank& bank, int& pinNumber);

public:

    enum class Bank
    {
        A,
        B,
    };

    enum class IoPin : int
    {
        GPA0 = 21,
        GPA1 = 22,
        GPA2 = 23,
        GPA3 = 24,
        GPA4 = 25,
        GPA5 = 26,
        GPA6 = 27,
        GPA7 = 28,
        
        GPB0 = 1,
        GPB1 = 2,
        GPB2 = 3,
        GPB3 = 4,
        GPB4 = 5,
        GPB5 = 6,
        GPB6 = 7,
        GPB7 = 8
    };

    enum class IoPinOutputState : bool
    {
        low = false,
        high = true
    };

    Mcp23017(I2cMultiplexedChannel* i2c_multiplexed_channel, int i2c_address);



    void set_pin_as_output(Bank bank, int pinNumber);
    void set_pin_as_output(IoPin pin);

    void set_pin_state(Bank bank, int pinNumber, bool highLow);
    void set_pin_state(const IoPin pin, const IoPinOutputState state);
};
