#include <cstdio>
#include <hardware/gpio.h>
#include <pico/time.h>

#include "i2cCommandReceiver.h"
#include "registers.h"

I2cCommandReceiver::I2cCommandReceiver(i2c_inst_t* i2c, Chassis* chassis)
{
    this->_i2c_instance = i2c;
    this->_chassis = chassis;
    I2cCommandReceiver::_instance = this;
};

void I2cCommandReceiver::command_receiver_init(uint sda_pin, uint scl_pin, uint baudrate, uint8_t slave_address)
{
    gpio_init(sda_pin);
    gpio_pull_up(sda_pin);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);

    gpio_init(scl_pin);
    gpio_pull_up(scl_pin);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    
    i2c_init(_i2c_instance, baudrate);
    i2c_slave_init(_i2c_instance, slave_address, i2c_slave_isr);
};

void I2cCommandReceiver::i2c_slave_isr(i2c_inst_t *i2c, i2c_slave_event_t event)
{
    switch (event)
    {
        case I2C_SLAVE_RECEIVE: // master has initiated a connection
            if(!I2cCommandReceiver::i2c_buffer.is_open()) // reads and writes always start with the memory address
            {
                uint8_t address = i2c_read_byte_raw(i2c);
                I2cCommandReceiver::i2c_buffer.begin_message(address);
            }
            else // master is writing data
            {
                uint8_t data = i2c_read_byte_raw(i2c);
                I2cCommandReceiver::i2c_buffer.add_data(data);
            }
            break;

        case I2C_SLAVE_REQUEST: // master is requesting data
        
            uint8_t read_address;
            read_address = I2cCommandReceiver::i2c_buffer.get_next_read_address();
            i2c_write_byte_raw(i2c, read_address);
            break;

        case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
            I2cCommandReceiver::i2c_buffer.end_message();
            I2cCommandReceiver::_instance->commit_buffer();
            break;

        default:
            break;
    }
};

void I2cCommandReceiver::commit_buffer()
{
    this->debug_buffer();
};

void I2cCommandReceiver::debug_buffer()
{
    printf("\n\n");
    uint8_t bytes_written;
    uint8_t* buffer = I2cCommandReceiver::i2c_buffer.get_written_bytes(&bytes_written);

    printf("%d Bytes written, starting at address: %d\n", bytes_written, I2cCommandReceiver::i2c_buffer.get_start_address());
    for(int i = 0; i < bytes_written; i++)
    {
        printf("Register: ??.  Value: %d\n", buffer[i]);
    }
    printf("\n\n");
};
