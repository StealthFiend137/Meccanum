#include <algorithm>
#include "i2cBuffer.h"

I2cBuffer::I2cBuffer()
{
    this->_registerMap[WDIR] = ModifiedRegisters::wAxisModified;
    this->_registerMap[XDIR] = ModifiedRegisters::xAxisModified;
    this->_registerMap[YDIR] = ModifiedRegisters::yAxisModified;
};

void I2cBuffer::begin_message(uint8_t register_address)
{
    this->_start_address = register_address;
    this->_address_written = true;
    this->_bytes_written = 0;
    this->_modified_registers = ModifiedRegisters::none;
};

void I2cBuffer::end_message()
{
    this->_address_written = false;
};

void I2cBuffer::add_data(uint8_t data)
{
    uint8_t addressBeingWritten = this->_start_address + this->_bytes_written;

    int writingIndex = this->_bytes_written;
    this->_bytes_written = this->_bytes_written + 1;

    this->_data[writingIndex] = data;
    this->_addresses[writingIndex] = addressBeingWritten;

    this->_modified_registers = this->_modified_registers | (I2cBuffer::ModifiedRegisters)_registerMap[addressBeingWritten];
};

bool I2cBuffer::is_open()
{
    return true == this->_address_written;
};

uint8_t I2cBuffer::get_start_address()
{
    return this->_start_address;
};

uint8_t* I2cBuffer::get_written_bytes(int* bytes_written)
{
    *bytes_written = std::max(this->_bytes_written, REGISTER_COUNT);
    return _data;
};

uint8_t I2cBuffer::get_next_read_address()
{
    uint8_t address = this->_start_address + _bytes_written;
    address %= REGISTER_COUNT;
    
    this->_bytes_written++;
    return address;
};
