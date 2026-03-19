#pragma once
#include "errors.h"

struct SensorSample {
    int flex1;
    int flex2;
    float angleY;
};

bool sensors_init();
SensorSample sensors_read();
ErrorCode sensors_init_checked();
// ErrorCode initMpu(uint8_t status);