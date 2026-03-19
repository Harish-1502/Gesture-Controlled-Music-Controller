#pragma once

#include "sensors.h"
#include "gesture_types.h"

void gestures_init();
GestureEvent gestures_detect(const SensorSample& sample);