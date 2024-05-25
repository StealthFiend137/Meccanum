    #pragma once
    #include "movement.h"
    #include "pico/time.h"
    
    void Movement::set_all_speeds(int x, int y, int w)
    {
        int start_time = to_ms_since_boot(get_absolute_time());
        xAxis.set_speed(x, start_time);
        yAxis.set_speed(y, start_time);
        wAxis.set_speed(w, start_time);
    };
