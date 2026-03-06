#pragma once

struct SensorSample {
    int flex1;
    int flex2;
    float angleY;
};

bool sensors_init();
SensorSample sensors_read();