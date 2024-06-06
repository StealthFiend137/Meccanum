#pragma once
#include "controlPin.h"
#include <mcp23017.h>

namespace ControlPins
{
    class Mcp23017_ControlPin;
}

class Mcp23017_ControlPin : public virtual ControlPin::ControlPin
{
public:

    enum class Bank
    {
        A,
        B,
    };

    Mcp23017_ControlPin::Mcp23017_ControlPin(MCP23017* extender, Bank bank, int PinNumber);
    void SetPinstate(ControlPin::PinState newPinState);
};
