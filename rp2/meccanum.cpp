#include "meccanum.h"

Meccanum::Meccanum(Chassis* chassis, Motors::Motor* frontLeft, Motors::Motor* frontRight, Motors::Motor* rearLeft, Motors::Motor* rearRight):
    _chassis(chassis), _frontLeft(frontLeft), _frontRight(frontRight), _rearLeft(rearLeft), _rearRight(rearRight)
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
    _frontLeft->set_speed(100);
    _frontRight->set_speed(100);
    _rearLeft->set_speed(100);
    _rearRight->set_speed(100);
};
