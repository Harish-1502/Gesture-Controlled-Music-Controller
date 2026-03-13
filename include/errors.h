#pragma once
#include <stdint.h>

#include "gestures.h"

enum class ErrorCode : uint8_t {
    None = 0,

    // Fatal startup / hardware errors
    MpuInitFailed,
    CalibrationDataInvalid,

    // System / communication
    BleDisconnected,
    BleCommandFailed,

    // Power / state issues
    InvalidStateTransition,

    // Calibration process
    CalibrationInProgress,
    CalibrationBadCapture,
};

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