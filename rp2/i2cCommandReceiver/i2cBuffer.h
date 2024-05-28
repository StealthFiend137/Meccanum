#pragma once
#include <cstdint>
#include "registers.h"

class I2cBuffer
{
private:

    enum class ModifiedRegisters : uint8_t
    {
        none = 0,
        wAxisModified = 1,
        xAxisModified = 2,
        yAxisModified = 4
    };

    inline friend ModifiedRegisters operator | (const ModifiedRegisters a, const ModifiedRegisters b){
        return static_cast<I2cBuffer::ModifiedRegisters>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    };

    inline friend ModifiedRegisters operator & (const ModifiedRegisters a, const ModifiedRegisters b){
        return static_cast<I2cBuffer::ModifiedRegisters>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    };

    uint8_t _start_address;
    bool _address_written { false };

    uint8_t _data[REGISTER_COUNT];
    uint8_t _addresses[REGISTER_COUNT];
    uint8_t _bytes_written { 0 };
    ModifiedRegisters _modified_registers { ModifiedRegisters::none };

public:

    void begin_message(uint8_t address);
    void add_data(uint8_t _data);
    bool is_open();
    void end_message();
    
    uint8_t get_start_address();
    uint8_t* get_written_bytes(uint8_t* bytes_written);
    uint8_t get_next_read_address();
};
