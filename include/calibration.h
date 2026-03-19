#pragma once
#include "errors.h"
#include "calibration_types.h"

void startCalibration();
bool loadCalibration();
void saveCalibration();
void runCalibration();

const FlexThresholds& getFlexThresholds();
const FlexCalibrationRaw& getFlexCalibrationRaw();

int readFlexAvg(int flexPin, bool calibrationMode);
int calibration_interp(int relax, int bent, float pct);
ErrorCode runCalibrationChecked();
bool calibrationRequested();