#pragma once
#include <hardware/i2c.h>
#include <pico/i2c_slave.h>

#include "memoryRegister.h"
#include "registers.h"
#include "i2cBuffer.h"
#include "../chassis/chassis.h"

/// @brief Responsible for handling i2c communication and sending and receiving commands from the chassis.
class I2cCommandReceiver
{
private:

    static inline I2cCommandReceiver* _instance;
    static void i2c_slave_isr(i2c_inst_t *i2c, i2c_slave_event_t event);

    /// @brief Backng field for the hardware i2c instance.
    i2c_inst_t* _i2c_instance;
    Chassis* _chassis;
    I2cBuffer _i2c_buffer;

    void debug_buffer();
    void commit_buffer();

public:

    I2cCommandReceiver(i2c_inst_t* i2c, Chassis* chassis);
    void command_receiver_init(uint sda_pin, uint scl_pin, uint baudrate, uint8_t slave_address);
};
