#pragma once
#include "motor.h"
#include "../control/controlPin.h"

namespace Motors
{
    class OpenLoop;  
};

class Motors::OpenLoop : public virtual Motors::Motor
{
public:

    int _pwm_gpio;
    ControlPins::ControlPin* _clockwisePin = nullptr;
    ControlPins::ControlPin* _antiClockwisePin = nullptr;

    OpenLoop(int pwm_gpio, ControlPins::ControlPin* clockwisePin, ControlPins::ControlPin* antiClockwisePin);
    void set_speed(int speed_in_percent) override;

private:
    int map_velocity(int percent);
    bool get_direction(int percent);
};
