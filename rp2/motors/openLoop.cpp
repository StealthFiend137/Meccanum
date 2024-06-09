#include <cmath>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "openLoop.h"

#include <cstdio>

static void ConfigurePinForPwm(int pinNumber)
{
    uint slice_num = ::pwm_gpio_to_slice_num(pinNumber);   
    pwm_config config = ::pwm_get_default_config();
    ::pwm_config_set_clkdiv(&config, 4.f);
    ::pwm_init(slice_num, &config, true);
};

Motors::OpenLoop::OpenLoop(int pwm_gpio, ControlPins::DigitalControlPin* clockwisePin, ControlPins::DigitalControlPin* antiClockwisePin):
    _pwm_gpio(pwm_gpio), _clockwisePin(clockwisePin), _antiClockwisePin(antiClockwisePin)
{
    ConfigurePinForPwm(pwm_gpio);
};

int Motors::OpenLoop::map_velocity(int percent)
{
    const int inputMin = 0;
    const int inputMax = 100;
    const int outputMin = 0;
    const int outputMax = UINT16_MAX;

    int inputSpeed = ::abs(percent);

    int velocity = outputMin + (percent - inputMin) * (outputMax - outputMin) / (inputMax - inputMin);
    return ::abs(velocity);
};

bool Motors::OpenLoop::get_direction(int percent)
{
   return true;
}

void Motors::OpenLoop::set_speed(int speed_in_percent)
{
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

    this->_clockwisePin->set_pin_state(newClockwisePinState);
    this->_antiClockwisePin->set_pin_state(newAnticlockwisePinState);

    int velocity = map_velocity(speed_in_percent);


    printf("velocity: %d\n", velocity);

    ::gpio_set_function(this->_pwm_gpio, GPIO_FUNC_PWM);
    ::pwm_set_gpio_level(this->_pwm_gpio, velocity);
};
