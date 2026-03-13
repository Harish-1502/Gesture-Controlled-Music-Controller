#include <unity.h>
#include "errors.h"
#include "state_machine.h"
#include "calibration.h"
#include "gestures.h"

void test_initMpu_ok();
void test_initMpu_fail();

void test_validateCalibrationData_valid();
void test_validateCalibrationData_flex1_invalid();
void test_validateCalibrationData_flex2_invalid();

void test_validateCalibrationCapture_valid();
void test_validateCalibrationCapture_flex1_small_gap();
void test_validateCalibrationCapture_flex2_small_gap();

void test_canTransition_valid();
void test_canTransition_invalid_sleep_to_calibrating();

void test_requireNotCalibrating_blocks_calibrating();
void test_requireNotCalibrating_allows_idle();

void test_sendMediaCommand_none_event();
void test_sendMediaCommand_disconnected();
void test_sendMediaCommand_valid();

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_initMpu_ok);
    RUN_TEST(test_initMpu_fail);

    RUN_TEST(test_validateCalibrationData_valid);
    RUN_TEST(test_validateCalibrationData_flex1_invalid);
    RUN_TEST(test_validateCalibrationData_flex2_invalid);

    RUN_TEST(test_validateCalibrationCapture_valid);
    RUN_TEST(test_validateCalibrationCapture_flex1_small_gap);
    RUN_TEST(test_validateCalibrationCapture_flex2_small_gap);

    RUN_TEST(test_canTransition_valid);
    RUN_TEST(test_canTransition_invalid_sleep_to_calibrating);

    RUN_TEST(test_requireNotCalibrating_blocks_calibrating);
    RUN_TEST(test_requireNotCalibrating_allows_idle);

    RUN_TEST(test_sendMediaCommand_none_event);
    RUN_TEST(test_sendMediaCommand_disconnected);
    RUN_TEST(test_sendMediaCommand_valid);

    return UNITY_END();
}

void test_initMpu_ok() {
    TEST_ASSERT_EQUAL(ErrorCode::None, initMpu(0));
}

void test_initMpu_fail() {
    TEST_ASSERT_EQUAL(ErrorCode::MpuInitFailed, initMpu(1));
}

void test_validateCalibrationData_valid() {
    FlexCalibrationRaw raw{200, 800, 250, 850};
    TEST_ASSERT_EQUAL(ErrorCode::None, validateCalibrationData(raw));
}

void test_validateCalibrationData_flex1_invalid() {
    FlexCalibrationRaw raw{800, 700, 250, 850};
    TEST_ASSERT_EQUAL(ErrorCode::CalibrationDataInvalid, validateCalibrationData(raw));
}

void test_validateCalibrationData_flex2_invalid() {
    FlexCalibrationRaw raw{200, 800, 900, 850};
    TEST_ASSERT_EQUAL(ErrorCode::CalibrationDataInvalid, validateCalibrationData(raw));
}

void test_validateCalibrationCapture_valid() {
    FlexCalibrationRaw raw{200, 500, 250, 600};
    TEST_ASSERT_EQUAL(ErrorCode::None, validateCalibrationCapture(raw));
}

void test_validateCalibrationCapture_flex1_small_gap() {
    FlexCalibrationRaw raw{200, 250, 250, 600};
    TEST_ASSERT_EQUAL(ErrorCode::CalibrationBadCapture, validateCalibrationCapture(raw));
}

void test_validateCalibrationCapture_flex2_small_gap() {
    FlexCalibrationRaw raw{200, 500, 250, 280};
    TEST_ASSERT_EQUAL(ErrorCode::CalibrationBadCapture, validateCalibrationCapture(raw));
}

void test_canTransition_valid() {
    TEST_ASSERT_EQUAL(ErrorCode::None,
        canTransition(State::Idle, State::Calibrating));
}

void test_canTransition_invalid_sleep_to_calibrating() {
    TEST_ASSERT_EQUAL(ErrorCode::InvalidStateTransition,
        canTransition(State::Sleep, State::Calibrating));
}

void test_requireNotCalibrating_blocks_calibrating() {
    TEST_ASSERT_EQUAL(ErrorCode::CalibrationInProgress,
        requireNotCalibrating(State::Calibrating));
}

void test_requireNotCalibrating_allows_idle() {
    TEST_ASSERT_EQUAL(ErrorCode::None,
        requireNotCalibrating(State::Idle));
}

void test_sendMediaCommand_none_event() {
    TEST_ASSERT_EQUAL(ErrorCode::BleCommandFailed,
        sendMediaCommand(GestureEvent::None));
}

void test_sendMediaCommand_disconnected() {
    TEST_ASSERT_EQUAL(ErrorCode::BleDisconnected,
        sendMediaCommand(GestureEvent::PlayPause));
}

void test_sendMediaCommand_valid() {
    TEST_ASSERT_EQUAL(ErrorCode::None,
        sendMediaCommand(GestureEvent::PlayPause));
}