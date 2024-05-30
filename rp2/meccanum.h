#pragma once
#include <cstdio>
#include "chassis/chassis.h"

class Meccanum
{
public:

    Meccanum(Chassis* chassis);
    void action_updates();

private:

    Chassis* _chassis;
    static inline Meccanum* _instance;

    static void update_callback(Chassis::Modified modified);
    void update_values(Chassis::Modified modified);
    void action_movement();

    Chassis::Modified _modified { Chassis::Modified::none };

    int wAxis_new { 0 };
    int xAxis_new { 0 };
    int yAxis_new { 0 };

    int wAxis_current { 0 };
    int xAxis_current { 0 };
    int yAxis_current { 0 };
};
