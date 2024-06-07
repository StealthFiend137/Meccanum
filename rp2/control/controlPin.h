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

    virtual void SetPinState(PinState newPinState) = 0;
};
