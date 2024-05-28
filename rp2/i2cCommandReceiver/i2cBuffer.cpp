#include "i2cBuffer.h"
#include <algorithm>
#include <map>

#include <cstdio>

void I2cBuffer::begin_message(uint8_t register_address)
{
    this->_start_address = register_address;
    this->_address_written = true;
    this->_bytes_written = 0;
};

void I2cBuffer::end_message()
{
    this->_address_written = false;
};

void I2cBuffer::add_data(uint8_t _data)
{
    std::map<uint8_t, ModifiedRegisters> registerMap;
    registerMap[WDIR] = ModifiedRegisters::wAxisModified;
    registerMap[XDIR] = ModifiedRegisters::xAxisModified;
    registerMap[YDIR] = ModifiedRegisters::yAxisModified;
    
    uint8_t addressBeingWritten = this->_start_address + this->_bytes_written;
    addressBeingWritten %= REGISTER_COUNT;

    this->_modified_registers = this->_modified_registers | registerMap[addressBeingWritten];

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
    *bytes_written = std::clamp(this->_bytes_written, (uint8_t)0, (uint8_t)REGISTER_COUNT);
    return _data;
};

uint8_t I2cBuffer::get_next_read_address()
{
    uint8_t address = this->_start_address + _bytes_written;
    address %= REGISTER_COUNT;
    
    this->_bytes_written++;
    return address;
};
