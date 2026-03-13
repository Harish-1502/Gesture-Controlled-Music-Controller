#include "errors.h"
#include "calibration.h"
#include "state_machine.h"
// #include "gestures.h"
#include "ble_media.h"
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

ErrorCode validateCalibrationData(const FlexCalibrationRaw& raw) {
    if (raw.f1Relax >= raw.f1Bent) return ErrorCode::CalibrationDataInvalid;
    if (raw.f2Relax >= raw.f2Bent) return ErrorCode::CalibrationDataInvalid;
    return ErrorCode::None;
}

ErrorCode validateCalibrationCapture(const FlexCalibrationRaw& raw) {
    const int minGap = 250; // example threshold

    if ((raw.f1Bent - raw.f1Relax) < minGap) return ErrorCode::CalibrationBadCapture;
    if ((raw.f2Bent - raw.f2Relax) < minGap) return ErrorCode::CalibrationBadCapture;

    return ErrorCode::None;
}

ErrorCode canTransition(State current, State next) {
    if (current == State::Sleep && next == State::Calibrating) {
        return ErrorCode::InvalidStateTransition;
    }

    if (current == next) {
        return ErrorCode::None;
    }

    return ErrorCode::None;
}

ErrorCode requireNotCalibrating(State current) {
    if (current == State::Calibrating) {
        return ErrorCode::CalibrationInProgress;
    }
    return ErrorCode::None;
}

ErrorCode sendMediaCommand(GestureEvent event){
    if (event == GestureEvent::None) {
        return ErrorCode::BleCommandFailed;
    }

    if (!ble_media_is_connected()) {
        return ErrorCode::BleDisconnected;
    }

    ble_media_send(event);
    return ErrorCode::None;
}

ErrorCode initMpu(uint8_t status){
    return (status == 0) ? ErrorCode::None : ErrorCode::MpuInitFailed;
}