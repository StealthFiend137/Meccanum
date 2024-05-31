#pragma once
#include <cstdio>
#include "motors/motor.h"
#include "chassis/chassis.h"


class Meccanum
{
public:

    Meccanum(Chassis* chassis, Motors::Motor* frontLeft, Motors::Motor* frontRight, Motors::Motor* rearLeft, Motors::Motor* rearRight);
    void action_updates();
    
private:

    Chassis* _chassis;
    static inline Meccanum* _instance;

    static void update_callback(Chassis::Modified modified);
    void update_values(Chassis::Modified modified);
    void action_movement();
    void calculateWheelVelocities(const int w, const int x, const int y, int &frontLeft, int &frontRight, int &rearLeft,int &rearRight);

    Chassis::Modified _modified { Chassis::Modified::none };

    int wAxis_new { 0 };
    int xAxis_new { 0 };
    int yAxis_new { 0 };

    int wAxis_current { 0 };
    int xAxis_current { 0 };
    int yAxis_current { 0 };

    Motors::Motor* _frontLeft = nullptr;
    Motors::Motor* _frontRight = nullptr;
    Motors::Motor* _rearLeft = nullptr;
    Motors::Motor* _rearRight = nullptr;
};
