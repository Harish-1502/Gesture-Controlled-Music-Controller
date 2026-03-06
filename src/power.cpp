#include "power.h"
#include "config.h"

#include <Arduino.h>
#include <OneButton.h>
#include <esp_sleep.h>

namespace {
    bool activated = false;
    OneButton button(PIN_POWER_BUTTON, true);

    void on_single_click() {
        activated = !activated;

        if (activated) {
            digitalWrite(PIN_LIGHT, HIGH);
            Serial.println("Device activated.");
        } else {
            digitalWrite(PIN_LIGHT, LOW);
            Serial.println("Device turned off.");
        }
    }

    void on_long_press_start() {
        Serial.println("Device going to sleep...");

        esp_sleep_enable_ext0_wakeup(GPIO_NUM_15, 0);

        while (digitalRead(PIN_POWER_BUTTON) == LOW) {
            delay(10);
        }

        esp_deep_sleep_start();
    }
}

void power_init() {
    pinMode(PIN_POWER_BUTTON, INPUT_PULLUP);
    pinMode(PIN_LIGHT, OUTPUT);
    digitalWrite(PIN_LIGHT, LOW);

    button.attachClick(on_single_click);
    button.attachLongPressStart(on_long_press_start);

    esp_sleep_wakeup_cause_t reason = esp_sleep_get_wakeup_cause();
    if (reason == ESP_SLEEP_WAKEUP_EXT0) {
        Serial.println("Woke from button press.");
    }
}

void power_update() {
    button.tick();
}

bool power_is_activated() {
    return activated;
}