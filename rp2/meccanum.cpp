#include "meccanum.h"

#include <cmath>
#include <algorithm>

/// @brief Initializes a new intance of the Meccanum class.
/// @param chassis 
/// @param frontLeft 
/// @param frontRight 
/// @param rearLeft 
/// @param rearRight 
Meccanum::Meccanum(Chassis* chassis, Motors::Motor* frontLeft, Motors::Motor* frontRight, Motors::Motor* rearLeft, Motors::Motor* rearRight):
    _chassis(chassis), _frontLeft(frontLeft), _frontRight(frontRight), _rearLeft(rearLeft), _rearRight(rearRight)
{
    chassis->register_callback(update_callback);

    frontLeft->set_speed(0);
    frontRight->set_speed(0);
    rearLeft->set_speed(0);
    rearRight->set_speed(0);

    Meccanum::_instance = this;
};

/// @brief 
/// @param modified 
void Meccanum::update_callback(Chassis::Modified modified)
{
    Meccanum::_instance->update_values(modified);
};

void Meccanum::update_values(Chassis::Modified modified)
{
    this->_modified = this->_modified | modified;

    if((modified & Chassis::Modified::wAxisModified) == Chassis::Modified::wAxisModified)
    {
        this->wAxis_new = this->_chassis->get_w_axis();
    };

    if((modified & Chassis::Modified::xAxisModified) == Chassis::Modified::xAxisModified)
    {
        this->xAxis_new = this->_chassis->get_x_axis();
    };

    if((modified & Chassis::Modified::yAxisModified) == Chassis::Modified::yAxisModified)
    {
        this->yAxis_new = this->_chassis->get_y_axis();
    };
};

void Meccanum::action_updates()
{
    if(Chassis::Modified::none == this->_modified) return;

    if(((this->_modified & Chassis::Modified::wAxisModified) == Chassis::Modified::wAxisModified)
    | ((this->_modified & Chassis::Modified::xAxisModified) == Chassis::Modified::xAxisModified)
    | ((this->_modified & Chassis::Modified::yAxisModified) == Chassis::Modified::yAxisModified))
    {
        this->wAxis_current = wAxis_new;
        this->xAxis_current = xAxis_new;
        this->yAxis_current = yAxis_new;
        this->action_movement();
    }

    this->_modified = Chassis::Modified::none;
};

void Meccanum::action_movement()
{

    int fl, fr, rl, rr;
    calculateWheelVelocities(this->wAxis_current, this->xAxis_current, this->yAxis_current, fl, fr, rl, rr);

    _frontLeft->set_speed(fl);
    _frontRight->set_speed(fr);
    _rearLeft->set_speed(rl);
    _rearRight->set_speed(rr);
};

void Meccanum::calculateWheelVelocities(const int w, const int x, const int y, int &frontLeft, int &frontRight, int &rearLeft,int &rearRight)
{
    double lx = 0.5;
    double wx = 0.5;

    double v_fl = y + x - w * (lx+wx);
    double v_fr = y - x - w * (lx+wx);
    double v_rl = y - x + w * (lx+wx);
    double v_rr = y + x + w * (lx+wx);

    double max = std::max(std::abs(v_fl), std::abs(v_fr));
    max = std::max(max, std::abs(v_rl));
    max = std::max(max, std::abs(v_rr));

    if(max > 100)
    {
        v_fl = (v_fl / max) * 100;
        v_fr = (v_fr / max) * 100;
        v_rl = (v_rl / max) * 100;
        v_rr = (v_rr / max) * 100;
    }

    frontLeft = v_fl;
    frontRight = v_fr;
    rearLeft = v_rl;
    rearRight = v_rr;
};
