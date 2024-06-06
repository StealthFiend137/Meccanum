#pragma once

namespace ControlPins
{
    class ControlPin;
};

class ControlPin
{
public:

    enum class PinState : bool
    {
        High = true,
        Low = false,
    };

    virtual PinState GetPinstate() const = 0;
};
