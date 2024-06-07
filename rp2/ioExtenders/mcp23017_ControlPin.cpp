#include "mcp23017_ControlPin.h"
    
ControlPins::Mcp23017_ControlPin::Mcp23017_ControlPin(IoExtenders::Mcp23017* extender, IoExtenders::Mcp23017::Bank bank, int pinNumber):
    _extender(extender), _bank(bank), _pinNumber(pinNumber)
{
    extender->set_pin_as_output(bank, pinNumber);
};

void ControlPins::Mcp23017_ControlPin::set_pin_state(ControlPins::DigitalControlPin::PinState newPinState)
{
    auto highLow = ControlPins::DigitalControlPin::PinState::High == newPinState;
    this->_extender->set_pin_state(this->_bank, this->_pinNumber, highLow);
};
