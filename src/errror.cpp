#include "errors.h"
#include <Arduino.h>

const char* errorToString(ErrorCode error) {
    switch (error) {
        case ErrorCode::None: return "No error";

        case ErrorCode::MpuInitFailed: return "MPU initialization failed";
        case ErrorCode::CalibrationDataInvalid: return "Calibration data invalid";

        case ErrorCode::BleDisconnected: return "BLE disconnected";
        case ErrorCode::BleCommandFailed: return "BLE command failed";

        case ErrorCode::InvalidStateTransition: return "Invalid state transition";

        case ErrorCode::CalibrationInProgress: return "Calibration in progress";
        case ErrorCode::CalibrationBadCapture: return "Bad data capture during calibration";

        default: return "Unknown error code";
    }
}

static ErrorCode currentError = ErrorCode::None;

ErrorCode getCurrentError() {
    return currentError;
}

void setCurrentError(ErrorCode error) {
    if (currentError != error) {
        currentError = error;
        Serial.printf("Error set: %s\n", errorToString(error));
    }
}

void errorClear() {
    setCurrentError(ErrorCode::None);
}

bool errorIsFatal(ErrorCode error) {
    switch (error) {
        case ErrorCode::MpuInitFailed:
            return true;

        default:
            return false;
    }
}

bool errorRequiresCalibration(ErrorCode error) {
    switch (error) {
        case ErrorCode::CalibrationDataInvalid:
        case ErrorCode::CalibrationInProgress:
        case ErrorCode::CalibrationBadCapture:
            return true;

        default:
            return false;
    }
}