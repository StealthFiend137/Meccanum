#pragma once

namespace ControlPins
{
    class ControlPin;
};

class ControlPins::ControlPin
{
public:

    enum class PinState : bool
    {
        High = true,
        Low = false,
    };

    virtual void SetPinState(PinState newPinState) = 0;
};
