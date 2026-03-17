#include <Arduino.h>

#include "sensors.h"
#include "gestures.h"
#include "ble_media.h"
#include "power.h"
#include "calibration.h"
#include "state_machine.h"
#include "errors.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  power_init();
  ble_media_init();
  sensors_init();
  startCalibration();
  gestures_init();

  if (!loadCalibration() || calibrationRequested()) {
    setCurrentError(ErrorCode::CalibrationDataInvalid);
    errorToString(getCurrentError());
    runCalibration();
  }
}

void loop() {
  power_update();

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

        if (event != GestureEvent::None && ble_media_is_connected()) {
            ble_media_send(event);
        } else if(!ble_media_is_connected()) {
            setCurrentError(ErrorCode::BleDisconnected);
            errorToString(getCurrentError());
            ble_media_init(); // Attempt to reinitialize BLE if disconnected
            errorClear(); // Clear error after attempting reconnection
        }
        break;
      }

      case State::Calibrating:{
        // Serial.println("Calibrating...");
        runCalibration();
        break;
      }

      case State::Sleep:{
        // Serial.println("Sleeping...");
        enter_sleep_mode();
        break;
      }
  }
}