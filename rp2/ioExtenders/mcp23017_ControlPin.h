#pragma once
#include "../control/controlPin.h"
#include "mcp23017.h"

namespace ControlPins
{
    class Mcp23017_ControlPin;
};

class ControlPins::Mcp23017_ControlPin : public virtual ControlPins::ControlPin
{
private:

    IoExtenders::Mcp23017* _extender;
    IoExtenders::Mcp23017::Bank _bank;
    int _pinNumber;

public:

    Mcp23017_ControlPin(IoExtenders::Mcp23017* extender, IoExtenders::Mcp23017::Bank bank, int PinNumber);
    void SetPinState(ControlPins::ControlPin::PinState newPinState) override;
};
