#include "mcp23017_ControlPin.h"
    
ControlPins::Mcp23017_ControlPin::Mcp23017_ControlPin(IoExtenders::Mcp23017* extender, IoExtenders::Mcp23017::Bank bank, int pinNumber):
    _extender(extender), _bank(bank), _pinNumber(pinNumber)
{
    
};

void ControlPins::Mcp23017_ControlPin::SetPinState(ControlPins::ControlPin::PinState newPinState)
{
    //this->_extender->SetPinState(this->_bank, this->_pinNumber);
};
