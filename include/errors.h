#pragma once
#include <stdint.h>
#include "state_machine.h"
#include "error_codes.h"
#include "gesture_types.h"
#include "calibration_types.h"

void setCurrentError(ErrorCode code);
void errorClear();

const char* errorToString(ErrorCode error);
bool errorIsFatal(ErrorCode error);
bool errorRequiresCalibration(ErrorCode error);

ErrorCode getCurrentError();
ErrorCode validateCalibrationData(const FlexCalibrationRaw& raw);
ErrorCode validateCalibrationCapture(const FlexCalibrationRaw& raw);
ErrorCode canTransition(State current, State next);
ErrorCode requireNotCalibrating(State current);
ErrorCode sendMediaCommand(GestureEvent event);
ErrorCode initMpu(uint8_t status);