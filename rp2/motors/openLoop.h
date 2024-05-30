#include "motor.h"

class OpenLoop : public virtual Motor::Motor
{
public:

    enum class Orientation
    {
        clockwise,
        anticlockwise
    };

    int _pwm_gpio;
    int _direction_forwards_gpio;
    int _direction_reverse_gpio;
    Orientation _orientation;

    OpenLoop(int pwm_gpio, int direction_forwards_gpio, int direction_reverse_gpio, Orientation Orientation);
    void set_speed(int speed_in_percent) override;
};





