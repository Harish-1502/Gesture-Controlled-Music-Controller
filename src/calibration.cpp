#include "calibration.h"
#include "config.h"

#include <Arduino.h>
#include <Preferences.h>

namespace {
    Preferences pref;

    FlexCalibrationRaw cal{};
    FlexThresholds thresholds{};

    float ema1 = 0.0f;
    float ema2 = 0.0f;

    void flash_led() {
        digitalWrite(PIN_LIGHT, HIGH);
        delay(50);
        digitalWrite(PIN_LIGHT, LOW);
    }

    void compute_thresholds() {
        thresholds.f1On  = calibration_interp(cal.f1Relax, cal.f1Bent, 0.90f);
        thresholds.f2On  = calibration_interp(cal.f2Relax, cal.f2Bent, 0.90f);

        thresholds.f1Off = static_cast<int>(thresholds.f1On * 0.85f);
        thresholds.f2Off = static_cast<int>(thresholds.f2On * 0.85f);
    }
}

void calibration_init() {
    pinMode(PIN_LIGHT, OUTPUT);
}

int calibration_interp(int relax, int bent, float pct) {
    return static_cast<int>(relax + pct * (bent - relax));
}

int calibration_read_flex_avg(int flexPin, bool calibrationMode) {
    int values[5];

    for (int i = 0; i < 5; i++) {
        values[i] = analogRead(flexPin);
        delay(5);
    }

    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (values[j] < values[i]) {
                int temp = values[i];
                values[i] = values[j];
                values[j] = temp;
            }
        }
    }

    int median = values[5 / 2];

    if (calibrationMode) {
        return median;
    }

    if (flexPin == PIN_FLEX1) {
        ema1 = 0.7f * ema1 + 0.3f * median;
        return static_cast<int>(ema1);
    } else {
        ema2 = 0.7f * ema2 + 0.3f * median;
        return static_cast<int>(ema2);
    }
}

void calibration_save() {
    pref.begin("mp3-controller", false);

    pref.putInt("f1Relax", cal.f1Relax);
    pref.putInt("f1Bent",  cal.f1Bent);
    pref.putInt("f2Relax", cal.f2Relax);
    pref.putInt("f2Bent",  cal.f2Bent);

    pref.end();
}

bool calibration_load() {
    pref.begin("mp3-controller", true);

    bool ok =
        pref.isKey("f1Relax") &&
        pref.isKey("f1Bent")  &&
        pref.isKey("f2Relax") &&
        pref.isKey("f2Bent");

    if (ok) {
        cal.f1Relax = pref.getInt("f1Relax");
        cal.f1Bent  = pref.getInt("f1Bent");
        cal.f2Relax = pref.getInt("f2Relax");
        cal.f2Bent  = pref.getInt("f2Bent");
        compute_thresholds();

        Serial.printf(
            "Loaded calibration. F1_ON:%d F1_OFF:%d F2_ON:%d F2_OFF:%d\n",
            thresholds.f1On, thresholds.f1Off,
            thresholds.f2On, thresholds.f2Off
        );
    }

    pref.end();
    return ok;
}

void calibration_run() {
    Serial.println("Starting calibration...");

    Serial.println("Step 1: Keep hand RELAXED (no bends)...");
    delay(5000);
    cal.f1Relax = calibration_read_flex_avg(PIN_FLEX1, true);
    cal.f2Relax = calibration_read_flex_avg(PIN_FLEX2, true);
    Serial.printf("Relaxed -> F1:%d F2:%d\n", cal.f1Relax, cal.f2Relax);
    flash_led();

    Serial.println("Step 2: Bend FLEX 1 fully...");
    delay(5000);
    cal.f1Bent = calibration_read_flex_avg(PIN_FLEX1, true);
    Serial.printf("F1 Bent -> %d\n", cal.f1Bent);
    flash_led();

    Serial.println("Step 3: Bend FLEX 2 fully...");
    delay(5000);
    cal.f2Bent = calibration_read_flex_avg(PIN_FLEX2, true);
    Serial.printf("F2 Bent -> %d\n", cal.f2Bent);
    flash_led();

    compute_thresholds();
    calibration_save();

    Serial.printf(
        "Saved calibration. F1_ON:%d F1_OFF:%d F2_ON:%d F2_OFF:%d\n",
        thresholds.f1On, thresholds.f1Off,
        thresholds.f2On, thresholds.f2Off
    );

    Serial.println("Calibration complete.\n");
}

const FlexCalibrationRaw& calibration_get_raw() {
    return cal;
}

const FlexThresholds& calibration_get_thresholds() {
    return thresholds;
}