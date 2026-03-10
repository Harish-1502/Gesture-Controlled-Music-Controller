#include <Arduino.h>

#include "sensors.h"
#include "gestures.h"
#include "ble_media.h"
#include "power.h"
#include "calibration.h"
#include "state_machine.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  power_init();
  ble_media_init();
  sensors_init();
  startCalibration();
  gestures_init();

  if (!loadCalibration() || calibrationRequested()) {
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