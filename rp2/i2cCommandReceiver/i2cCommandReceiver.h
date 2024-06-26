#pragma once
#include <hardware/i2c.h>
#include <pico/i2c_slave.h>

#include "i2cBuffer.h"
#include "../chassis/chassis.h"

/// @brief Responsible for handling i2c communication and sending and receiving commands from the chassis.
class I2cCommandReceiver : virtual public UpdatedValuesEventListener
{
private:

    static inline I2cCommandReceiver* _instance;
    static void i2c_slave_isr(i2c_inst_t *i2c, i2c_slave_event_t event);

    /// @brief Backng field for the hardware i2c instance.
    i2c_inst_t* _i2c_instance;
    Chassis* _chassis;
    I2cBuffer _i2c_buffer;

    void commit_buffer();

    bool get_if_register_modified(I2cBuffer::ModifiedRegisters effectedRegister);

public:

    I2cCommandReceiver(i2c_inst_t* i2c, Chassis* chassis);
    void command_receiver_init(uint sda_pin, uint scl_pin, uint baudrate, uint8_t slave_address);
    void ValuesUpdated(UpdatedValues updatedValues) override;
    
    static void update_notification_callback(Chassis::Modified modifed);
    void update_notification(Chassis::Modified modifed);
};
