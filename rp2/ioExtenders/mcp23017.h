#pragma once
#include <hardware/i2c.h>

namespace IoExtenders
{
    class Mcp23017;
};

class IoExtenders::Mcp23017
{
private:

    int _i2c_address;
    i2c_inst_t* _i2c_instance;

public:
    Mcp23017(i2c_inst_t* i2c_instance, int i2c_address);
};
