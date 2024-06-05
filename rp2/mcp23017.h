#pragma once
#include "hardware/i2c.h"

class MCP23017
{
private:

    int _i2c_address;
    i2c_inst_t* _i2c_instance;

public:
    MCP23017(i2c_inst_t* i2c_instance, int i2c_address);
};
