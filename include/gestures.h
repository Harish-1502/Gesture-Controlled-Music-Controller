#pragma once

#include "sensors.h"

enum class GestureEvent {
    None,
    PlayPause,
    VolumeUp,
    VolumeDown,
    NextTrack,
    PreviousTrack
};

void gestures_init();
GestureEvent gestures_detect(const SensorSample& sample);