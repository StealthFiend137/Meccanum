#pragma once
#include <cstdio>
#include "chassis/chassis.h"

class Meccanum
{
public:

    Meccanum(Chassis* chassis,
    int pwm_0_gpio, int direction_0_gpio,
    int pwm_1_gpio, int direction_1_gpio,
    int pwm_2_gpio, int direction_2_gpio,
    int pwm_3_gpio, int direction_3_gpio);
    
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

    int _pwm_0_gpio;
    int _direction_0_gpio;
    int _pwm_1_gpio;
    int _direction_1_gpio;
    int _pwm_2_gpio;
    int _direction_2_gpio;
    int _pwm_3_gpio;
    int _direction_3_gpio;
};
