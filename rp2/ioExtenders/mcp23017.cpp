#include "mcp23017.h"

/// @brief Provides control of the output pins of an MCP23017.
/// @param i2c_instance The already connected i2c instance.
/// @param i2c_address The address of the MCP23017.
IoExtenders::Mcp23017::Mcp23017(i2c_inst_t* i2c_instance, int i2c_address):
    _i2c_instance(i2c_instance), _i2c_address(i2c_address)
{
};
