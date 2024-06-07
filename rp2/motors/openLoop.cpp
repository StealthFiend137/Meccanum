#include <cstdio>
#include <cmath>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "openLoop.h"

Motors::OpenLoop::OpenLoop(int pwm_gpio, ControlPins::DigitalControlPin* clockwisePin, ControlPins::DigitalControlPin* antiClockwisePin):
    _pwm_gpio(pwm_gpio), _clockwisePin(clockwisePin), _antiClockwisePin(antiClockwisePin)
{
    gpio_set_function(pwm_gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pwm_gpio);
    pwm_config config = pwm_get_default_config();
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pwm_gpio, 0);
};

int Motors::OpenLoop::map_velocity(int percent)
{
    const int inputMin = -100;
    const int inputMax = 100;
    const int outputMin = -255;
    const int outputMax = 255;

    int velocity = outputMin + (percent - inputMin) * (outputMax - outputMin) / (inputMax - inputMin);
    return abs(velocity);
};

bool Motors::OpenLoop::get_direction(int percent)
{
   return true;
}

void Motors::OpenLoop::set_speed(int speed_in_percent)
{
    // bool direction = get_direction(speed_in_percent);
    auto newClockwisePinState = ControlPins::DigitalControlPin::PinState::Low;
    auto newAnticlockwisePinState = ControlPins::DigitalControlPin::PinState::Low;

    if(speed_in_percent > 0)
    {
        newClockwisePinState = ControlPins::DigitalControlPin::PinState::High;
    }
    else
    {
        newAnticlockwisePinState = ControlPins::DigitalControlPin::PinState::High;
    }

    this->_clockwisePin->SetPinState(newClockwisePinState);
    this->_antiClockwisePin->SetPinState(newAnticlockwisePinState);

    int velocity = map_velocity(speed_in_percent);
    pwm_set_gpio_level(this->_pwm_gpio, velocity);
};
