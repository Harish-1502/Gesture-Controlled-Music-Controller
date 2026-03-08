#include "calibration.h"
#include "config.h"

#include <Arduino.h>
#include <Preferences.h>
// #include <OneButton.h>

namespace {
    Preferences pref;

    FlexCalibrationRaw cal{};
    FlexThresholds thresholds{};

    float ema1 = 0.0f;
    float ema2 = 0.0f;

    void flash_led(int times) {
        for (int i = 0; i < times; i++)
        {
            digitalWrite(PIN_LIGHT, HIGH);
            delay(150);
            digitalWrite(PIN_LIGHT, LOW);
            delay(150);
        }
    }
    
    void countdown(int seconds)
    {
        for(int i = seconds; i > 0; i--)
        {
            Serial.printf("%d...\n", i);
            delay(1000);
        }
    }

    void compute_thresholds() {
        thresholds.f1On  = calibration_interp(cal.f1Relax, cal.f1Bent, 0.90f);
        thresholds.f2On  = calibration_interp(cal.f2Relax, cal.f2Bent, 0.90f);

        thresholds.f1Off = static_cast<int>(thresholds.f1On * 0.85f);
        thresholds.f2Off = static_cast<int>(thresholds.f2On * 0.85f);
    }
    
    
}
int calibration_interp(int relax, int bent, float pct) {
    return static_cast<int>(relax + pct * (bent - relax));
}

void startCalibration() {
    pinMode(PIN_LIGHT, OUTPUT);
    // button.attachLongPressStart(runCalibration);
}

int readFlexAvg(int flexPin, bool calibrationMode) {
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

void saveCalibration() {
    pref.begin("mp3-controller", false);

    pref.putInt("f1Relax", cal.f1Relax);
    pref.putInt("f1Bent",  cal.f1Bent);
    pref.putInt("f2Relax", cal.f2Relax);
    pref.putInt("f2Bent",  cal.f2Bent);

    pref.end();
}

bool loadCalibration() {
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

void runCalibration() {
    Serial.println("Starting calibration...");

    Serial.println("Step 1: Keep hand RELAXED (no bends)...");
    flash_led(3);
    delay(5000);
    countdown(3);
    cal.f1Relax = readFlexAvg(PIN_FLEX1, true);
    cal.f2Relax = readFlexAvg(PIN_FLEX2, true);
    Serial.printf("Relaxed -> F1:%d F2:%d\n", cal.f1Relax, cal.f2Relax);
    
    Serial.println("Step 2: Bend FLEX 1 fully...");
    flash_led(2);
    delay(5000);
    countdown(3);
    cal.f1Bent = readFlexAvg(PIN_FLEX1, true);
    Serial.printf("F1 Bent -> %d\n", cal.f1Bent);
    // flash_led(2);

    Serial.println("Step 3: Bend FLEX 2 fully...");
    flash_led(1);
    delay(5000);
    countdown(3);
    cal.f2Bent = readFlexAvg(PIN_FLEX2, true);
    Serial.printf("F2 Bent -> %d\n", cal.f2Bent);
    
    flash_led(5);
    compute_thresholds();
    saveCalibration();

    Serial.printf(
        "Saved calibration. F1_ON:%d F1_OFF:%d F2_ON:%d F2_OFF:%d\n",
        thresholds.f1On, thresholds.f1Off,
        thresholds.f2On, thresholds.f2Off
    );

    Serial.println("Calibration complete.\n");
}

const FlexCalibrationRaw& getFlexCalibrationRaw() {
    return cal;
}

const FlexThresholds& getFlexThresholds() {
    return thresholds;
}

bool calibrationRequested()
{
    return digitalRead(PIN_POWER_BUTTON) == LOW;
}