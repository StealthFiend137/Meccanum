#include <hardware/gpio.h>
#include <pico/time.h>

#include "i2cCommandReceiver.h"
#include "registers.h"

I2cCommandReceiver::I2cCommandReceiver(i2c_inst_t* i2c, Chassis* chassis)
{
    this->_i2c_instance = i2c;
    this->_chassis = chassis;
    this->_chassis->register_callback(update_notification_callback);
    I2cCommandReceiver::_instance = this;
};

void I2cCommandReceiver::update_notification_callback(Chassis::Modified modified)
{
    I2cCommandReceiver::_instance->update_notification(modified);
};

void I2cCommandReceiver::update_notification(Chassis::Modified modified)
{
};

void I2cCommandReceiver::ValuesUpdated(UpdatedValues updatedValues)
{
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
            if(!I2cCommandReceiver::_instance->_i2c_buffer.is_open()) // reads and writes always start with the memory address
            {
                uint8_t address = i2c_read_byte_raw(i2c);
                I2cCommandReceiver::_instance->_i2c_buffer.begin_message(address);
            }
       
     else // master is writing data
            {
                uint8_t data = i2c_read_byte_raw(i2c);
                I2cCommandReceiver::_instance->_i2c_buffer.add_data(data);
            }
            break;

        case I2C_SLAVE_REQUEST: // master is requesting data
        
            uint8_t read_address;
            read_address = I2cCommandReceiver::_instance->_i2c_buffer.get_next_read_address();
            i2c_write_byte_raw(i2c, read_address);
            break;

        case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
            I2cCommandReceiver::_instance->_i2c_buffer.end_message();
            I2cCommandReceiver::_instance->commit_buffer();
            break;

        default:
            break;
    }
};

bool I2cCommandReceiver::get_if_register_modified(I2cBuffer::ModifiedRegisters effectedRegister)
{
    return effectedRegister == (this->_i2c_buffer._modified_registers & effectedRegister);
};

uint8_t GetValueByAddress(uint8_t address, uint8_t* addresses, uint8_t* data) 
{
    for(int i = 0; i<REGISTER_COUNT; i++)
    {
        if(addresses[i] == address)
        {
            return data[i];
        };
    }

    return 0;
};

void I2cCommandReceiver::commit_buffer()
{
    bool wAxisModified = this->get_if_register_modified(I2cBuffer::ModifiedRegisters::wAxisModified);
    bool xAxisModified = this->get_if_register_modified(I2cBuffer::ModifiedRegisters::xAxisModified);
    bool yAxisModified = this->get_if_register_modified(I2cBuffer::ModifiedRegisters::yAxisModified);
    bool allAxisModifed = wAxisModified & xAxisModified & yAxisModified;

    if(allAxisModifed)
    {
        auto wAxisValue = (int)GetValueByAddress(WDIR, this->_i2c_buffer._addresses, this->_i2c_buffer._data);
        auto xAxisValue = (int)GetValueByAddress(XDIR, this->_i2c_buffer._addresses, this->_i2c_buffer._data);
        auto yAxisValue = (int)GetValueByAddress(YDIR, this->_i2c_buffer._addresses, this->_i2c_buffer._data);
        this->_chassis->set_all_axes(wAxisValue, xAxisValue, yAxisValue);
        return;
    }

    if(wAxisModified)
    {
        auto wAxisValue = (int)GetValueByAddress(WDIR, this->_i2c_buffer._addresses, this->_i2c_buffer._data);
        this->_chassis->set_w_axis(wAxisValue);
    }

    if(xAxisModified)
    {
        auto xAxisValue = (int)GetValueByAddress(XDIR, this->_i2c_buffer._addresses, this->_i2c_buffer._data);
        this->_chassis->set_x_axis(xAxisValue);
    }

    if(yAxisModified)
    {
        auto yAxisValue = (int)GetValueByAddress(YDIR, this->_i2c_buffer._addresses, this->_i2c_buffer._data);
        this->_chassis->set_y_axis(yAxisModified);
    }
};
