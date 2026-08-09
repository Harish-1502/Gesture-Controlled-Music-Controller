#include <Arduino.h>

#include "sensors.h"
#include "gestures.h"
#include "ble_media.h"
#include "power.h"
#include "calibration.h"
#include "state_machine.h"
#include "errors.h"
#include "recovery.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  power_init();
  ble_media_init();

  ErrorCode sensorErr = sensors_init_checked();
  if (sensorErr != ErrorCode::None) {
    handleError(sensorErr);
  }

  // startCalibration();
  gestures_init();

  // if (!loadCalibration() || calibrationRequested()) {
  //   handleError(ErrorCode::CalibrationDataInvalid);
  // }
}

void loop() {
  power_update();
  serviceRecovery();

  switch(getCurrentState())
  {
      case State::Idle:{
        // checkWakeGesture();
        // Serial.println("Idle...");
        break;
      }
      case State::Running:{
        // processGestures();
        // Serial.println("Running...");
        const SensorSample sample = sensors_read();

        const GestureEvent event = gestures_detect(sample);

        if (event != GestureEvent::None) {
          ErrorCode err = ble_media_send_checked(event);
          if (err != ErrorCode::None) {
            handleError(err);
          } else {
            errorClear();
          }
        }
        break;
      }

      case State::Calibrating:{
        // Serial.println("Calibrating...");
        ErrorCode err = runCalibrationChecked();
        if (err != ErrorCode::None) {
          handleError(err);
        } else {
          errorClear();
          setCurrentState(State::Running);
        }
        break;
      }

      case State::Sleep:{
        // Serial.println("Sleeping...");
        enter_sleep_mode();
        break;
      }
  }
}
