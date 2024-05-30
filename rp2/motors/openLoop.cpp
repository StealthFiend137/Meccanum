#include <cstdio>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "openLoop.h"

Motors::OpenLoop::OpenLoop(int pwm_gpio, int direction_gpio, Orientation orientation):
    _pwm_gpio(pwm_gpio), _direction_gpio(direction_gpio), _orientation(orientation)
{
    gpio_init(direction_gpio);
    gpio_set_dir(direction_gpio, GPIO_OUT);
    gpio_put(direction_gpio, true);

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

    return outputMin + (percent - inputMin) * (outputMax - outputMin) / (inputMax - inputMin);
};

void Motors::OpenLoop::set_speed(int speed_in_percent)
{
    bool direction;
    switch(this->_orientation)
    {
        case(Orientation::clockwise):
            direction = speed_in_percent > 0;
            break;

        case(Orientation::anticlockwise):
            direction = speed_in_percent < 0;
            break;
    }

    int velocity = map_velocity(speed_in_percent);

    gpio_put(this->_direction_gpio, direction);

    printf("Setting percent of %d speed to %d\n", speed_in_percent, velocity);
};