#pragma once

#include <Arduino.h>

// Pins
constexpr int PIN_POWER_BUTTON = 15;
constexpr int PIN_LIGHT        = 13;
constexpr int PIN_FLEX1        = 34;
constexpr int PIN_FLEX2        = 33;

// Flex thresholds
constexpr int FLEX1_TRIGGER = 1400;
constexpr int FLEX2_TRIGGER = 2200;

// Tilt thresholds
constexpr float TILT_RIGHT_ON  =  25.0f;
constexpr float TILT_LEFT_ON   = -25.0f;
constexpr float TILT_CENTER_OK =  10.0f;

// Cooldowns
constexpr uint32_t PLAY_PAUSE_COOLDOWN_MS = 700;
constexpr uint32_t VOL_COOLDOWN_MS        = 200;
constexpr uint32_t TRACK_COOLDOWN_MS      = 500;