#pragma once
#include <cstdint>
#include <map>
#include "registers.h"

class I2cBuffer
{
public:

    I2cBuffer();

    enum class ModifiedRegisters : uint8_t
    {
        none = 0,
        wAxisModified = 1,
        xAxisModified = 2,
        yAxisModified = 4
    };

private:

    uint8_t _start_address;
    bool _address_written { false };

    int _bytes_written;

    std::map<uint8_t, ModifiedRegisters> _registerMap;

public:

    uint8_t _data[REGISTER_COUNT];
    uint8_t _addresses[REGISTER_COUNT];
    ModifiedRegisters _modified_registers { ModifiedRegisters::none };

    friend I2cBuffer::ModifiedRegisters operator | (const I2cBuffer::ModifiedRegisters a, const I2cBuffer::ModifiedRegisters b){
        return static_cast<I2cBuffer::ModifiedRegisters>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    };

    friend I2cBuffer::ModifiedRegisters operator & (const I2cBuffer::ModifiedRegisters a, const I2cBuffer::ModifiedRegisters b){
        return static_cast<I2cBuffer::ModifiedRegisters>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    };

    void begin_message(uint8_t address);
    void add_data(uint8_t _data);
    bool is_open();
    void end_message();
    
    uint8_t get_start_address();
    uint8_t* get_written_bytes(int* bytes_written);
    uint8_t get_next_read_address();
};
