#pragma once

struct FlexCalibrationRaw
{
    int f1Relax;
    int f1Bent;
    int f2Relax;
    int f2Bent;
};

struct FlexThresholds
{
    int f1On;
    int f1Off;
    int f2On;
    int f2Off;
};

void startCalibration();
bool loadCalibration();
void saveCalibration();
void runCalibration();

const FlexThresholds& getFlexThresholds();
const FlexCalibrationRaw& getFlexCalibrationRaw();

int readFlexAvg(int flexPin, bool calibrationMode);
int calibration_interp(int relax, int bent, float pct);

bool calibrationRequested();