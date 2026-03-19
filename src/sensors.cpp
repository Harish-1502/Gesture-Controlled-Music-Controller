#include "sensors.h"
#include "config.h"
#include "errors.h"

#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_light.h>

static MPU6050 mpu(Wire);

bool sensors_init() {
    Wire.begin();

    byte status = mpu.begin();
    Serial.print(F("MPU6050 status: "));
    Serial.println(status);

    ErrorCode error = initMpu(status);

    if (error == ErrorCode::MpuInitFailed) {
        setCurrentError(ErrorCode::MpuInitFailed);
        Serial.println(errorToString(getCurrentError()));
        return false;
    }

    Serial.println(F("Calculating offsets, do not move MPU6050"));
    delay(1000);
    mpu.calcOffsets(true, true);
    Serial.println(F("Done!\n"));

    analogSetPinAttenuation(PIN_FLEX1, ADC_11db);
    analogSetPinAttenuation(PIN_FLEX2, ADC_11db);

    return true;
}

SensorSample sensors_read() {
    mpu.update();

    SensorSample sample{};
    sample.flex1  = analogRead(PIN_FLEX1);
    sample.flex2  = analogRead(PIN_FLEX2);
    sample.angleY = mpu.getAngleY();

    return sample;
}

ErrorCode sensors_init_checked() {
    Wire.begin();

    byte status = 0;

    #ifdef INJECT_MPU_INIT_FAIL
        status = 1;
    #else
        status = mpu.begin();
    #endif

    Serial.print(F("MPU6050 status: "));
    Serial.println(status);

    ErrorCode err = initMpu(status);
    if (err != ErrorCode::None) {
        return err;
    }

    Serial.println(F("Calculating offsets, do not move MPU6050"));
    delay(1000);
    mpu.calcOffsets(true, true);
    Serial.println(F("Done!\n"));

    analogSetPinAttenuation(PIN_FLEX1, ADC_11db);
    analogSetPinAttenuation(PIN_FLEX2, ADC_11db);

    return ErrorCode::None;
}