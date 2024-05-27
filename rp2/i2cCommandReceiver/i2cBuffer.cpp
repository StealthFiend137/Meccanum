#include "i2cBuffer.h"
#include <cstdio>

void I2cBuffer::begin_message(uint8_t register_address)
{
    printf("Begin message\n");
    this->_start_address = register_address;
    this->_address_written = true;
    this->_bytes_written = 0;
};

void I2cBuffer::end_message()
{
    printf("End message\n");
    this->_address_written = false;
};

void I2cBuffer::add_data(uint8_t _data)
{
    this->_data[this->_bytes_written] = _data;
    this->_bytes_written++;
};

bool I2cBuffer::is_open()
{
    return true == this->_address_written;
};

uint8_t I2cBuffer::get_start_address()
{
    return this->_start_address;
};

uint8_t* I2cBuffer::get_written_bytes(uint8_t* bytes_written)
{
    *bytes_written = this->_bytes_written;
    return _data;
};
