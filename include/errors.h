#pragma once
#include <stdint.h>

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
ErrorCode getCurrentError();

const char* errorToString(ErrorCode error);
bool errorIsFatal(ErrorCode error);
bool errorRequiresCalibration(ErrorCode error);