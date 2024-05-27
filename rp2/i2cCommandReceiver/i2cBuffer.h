#pragma once
#include <cstdint>
#include "registers.h"

class I2cBuffer
{
private:

    uint8_t _start_address;
    bool _address_written = false;

    uint8_t _data[REGISTER_COUNT];
    uint8_t _bytes_written = 0;

public:

    void begin_message(uint8_t address);
    void add_data(uint8_t _data);
    bool is_open();
    void end_message();
    
    uint8_t get_start_address();
    uint8_t* get_written_bytes(uint8_t* bytes_written);
};
