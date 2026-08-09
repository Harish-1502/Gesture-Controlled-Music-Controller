#include <Arduino.h>
#include <OneButton.h>

constexpr int PIN_POWER_BUTTON = 13;
constexpr int PIN_LIGHT = 15;

OneButton button(PIN_POWER_BUTTON, true);

void onClick() {
    Serial.println("CLICK detected");
    digitalWrite(PIN_LIGHT, !digitalRead(PIN_LIGHT));
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(PIN_POWER_BUTTON, INPUT_PULLUP);
    pinMode(PIN_LIGHT, OUTPUT);
    digitalWrite(PIN_LIGHT, LOW);

    button.attachClick(onClick);

    Serial.println("Button test started");
}

void loop() {
    button.tick();

    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 2000) {
        Serial.print("Raw button = ");
        Serial.println(digitalRead(PIN_POWER_BUTTON));
        lastPrint = millis();
    }
}
