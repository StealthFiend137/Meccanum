#include "meccanum.h"

Meccanum::Meccanum(Chassis* chassis,
    int pwm_0_gpio, int direction_0_gpio,
    int pwm_1_gpio, int direction_1_gpio,
    int pwm_2_gpio, int direction_2_gpio,
    int pwm_3_gpio, int direction_3_gpio):
    _chassis(chassis),
    _pwm_0_gpio(pwm_0_gpio), _direction_0_gpio(direction_0_gpio),
    _pwm_1_gpio(pwm_1_gpio), _direction_1_gpio(direction_1_gpio),
    _pwm_2_gpio(pwm_2_gpio), _direction_2_gpio(direction_2_gpio),
    _pwm_3_gpio(pwm_3_gpio), _direction_3_gpio(direction_3_gpio)
{
    chassis->register_callback(update_callback);
    Meccanum::_instance = this;
};

void Meccanum::update_callback(Chassis::Modified modified)
{
    Meccanum::_instance->update_values(modified);
};

void Meccanum::update_values(Chassis::Modified modified)
{
    this->_modified = this->_modified | modified;

    if((modified & Chassis::Modified::wAxisModified) == Chassis::Modified::wAxisModified)
    {
        wAxis_new = this->_chassis->get_w_axis();
    };

    if((modified & Chassis::Modified::xAxisModified) == Chassis::Modified::xAxisModified)
    {
        xAxis_new = this->_chassis->get_x_axis();
    };

    if((modified & Chassis::Modified::yAxisModified) == Chassis::Modified::yAxisModified)
    {
        yAxis_new = this->_chassis->get_y_axis();
    };
};

void Meccanum::action_updates()
{
    if(Chassis::Modified::none == this->_modified) return;

    if(((this->_modified & Chassis::Modified::wAxisModified) == Chassis::Modified::wAxisModified)
    | ((this->_modified & Chassis::Modified::xAxisModified) == Chassis::Modified::xAxisModified)
    | ((this->_modified & Chassis::Modified::yAxisModified) == Chassis::Modified::yAxisModified))
    {
        wAxis_current = wAxis_new;
        xAxis_current = xAxis_new;
        yAxis_current = yAxis_new;
        this->action_movement();
    }

    this->_modified = Chassis::Modified::none;
};

void Meccanum::action_movement()
{
    printf("Axis Modified. %d, %d, %d\n", wAxis_current, xAxis_current, yAxis_current);
};
