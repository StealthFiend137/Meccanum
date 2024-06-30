#pragma once

namespace ControlPins
{
    class DigitalControlPin;
};

class ControlPins::DigitalControlPin
{
public:

    enum class PinState : bool
    {
        High = true,
        Low = false,
    };

    virtual void set_pin_state(PinState newPinState) = 0;
};
