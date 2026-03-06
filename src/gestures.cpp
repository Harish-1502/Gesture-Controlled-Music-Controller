#include "gestures.h"
#include "config.h"

#include <Arduino.h>
#include <math.h>

namespace {
    enum class TiltZone {
        Center,
        Right,
        Left
    };

    TiltZone tiltZone = TiltZone::Center;

    uint32_t lastPlayPauseMs = 0;
    uint32_t lastVolUpMs     = 0;
    uint32_t lastVolDownMs   = 0;
    uint32_t lastTrackCmdMs  = 0;

    bool cooldown(uint32_t& lastTime, uint32_t gapMs) {
        const uint32_t now = millis();

        if (now - lastTime < gapMs) {
            return false;
        }

        lastTime = now;
        return true;
    }
}

void gestures_init() {
    tiltZone = TiltZone::Center;
    lastPlayPauseMs = 0;
    lastVolUpMs = 0;
    lastVolDownMs = 0;
    lastTrackCmdMs = 0;
}

GestureEvent gestures_detect(const SensorSample& sample) {
    if (sample.flex2 >= FLEX2_TRIGGER &&
        sample.flex1 >= FLEX1_TRIGGER &&
        cooldown(lastPlayPauseMs, PLAY_PAUSE_COOLDOWN_MS)) {
        return GestureEvent::PlayPause;
    }

    if (sample.flex2 >= FLEX2_TRIGGER &&
        cooldown(lastVolUpMs, VOL_COOLDOWN_MS)) {
        return GestureEvent::VolumeUp;
    }

    if (sample.flex1 >= FLEX1_TRIGGER &&
        cooldown(lastVolDownMs, VOL_COOLDOWN_MS)) {
        return GestureEvent::VolumeDown;
    }

    if (sample.angleY > TILT_RIGHT_ON) {
        if (tiltZone != TiltZone::Right &&
            cooldown(lastTrackCmdMs, TRACK_COOLDOWN_MS)) {
            tiltZone = TiltZone::Right;
            return GestureEvent::NextTrack;
        }

        tiltZone = TiltZone::Right;
    }
    else if (sample.angleY < TILT_LEFT_ON) {
        if (tiltZone != TiltZone::Left &&
            cooldown(lastTrackCmdMs, TRACK_COOLDOWN_MS)) {
            tiltZone = TiltZone::Left;
            return GestureEvent::PreviousTrack;
        }

        tiltZone = TiltZone::Left;
    }
    else if (fabs(sample.angleY) < TILT_CENTER_OK) {
        tiltZone = TiltZone::Center;
    }

    return GestureEvent::None;
}