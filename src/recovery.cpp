#include "recovery.h"

#include <Arduino.h>

#include "state_machine.h"
#include "ble_media.h"
#include "errors.h"

static unsigned long lastBleRecoveryAttemptMs = 0;
static const unsigned long BLE_RECOVERY_INTERVAL_MS = 3000;

void handleError(ErrorCode err) {
    if (err == ErrorCode::None) {
        return;
    }

    setCurrentError(err);
    Serial.println(errorToString(err));

    switch (err) {
        case ErrorCode::MpuInitFailed:
            setCurrentState(State::Idle);
            break;

        case ErrorCode::CalibrationDataInvalid:
        case ErrorCode::CalibrationBadCapture:
            setCurrentState(State::Calibrating);
            break;

        case ErrorCode::BleDisconnected:
            // stay in Running, recovery handled separately
            break;

        case ErrorCode::BleCommandFailed:
            // ignore failed command, keep running
            break;

        case ErrorCode::InvalidStateTransition:
            // stay in current state
            break;

        case ErrorCode::CalibrationInProgress:
            // block normal actions while calibrating
            break;

        case ErrorCode::None:
        default:
            break;
    }
}

void serviceRecovery() {
    if (getCurrentError() == ErrorCode::BleDisconnected) {
        const unsigned long now = millis();

        if (now - lastBleRecoveryAttemptMs >= BLE_RECOVERY_INTERVAL_MS) {
            lastBleRecoveryAttemptMs = now;

            ble_media_init();

            if (ble_media_is_connected()) {
                errorClear();
                Serial.println("BLE recovered");
            }
        }
    }
}