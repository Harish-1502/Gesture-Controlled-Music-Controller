#include "state_machine.h"
#include <Arduino.h>

namespace{
    const char* stateToString(State s)
    {
        switch (s)
        {
            case State::Running:     return "Running";
            case State::Calibrating: return "Calibrating";
            case State::Idle:        return "Idle";
            case State::Sleep:       return "Sleep";
            default:                 return "Unknown";
        }
    }
}

static State currentState = State::Idle;

State getCurrentState() {
    return currentState;
}

void setCurrentState(State newState)
{
    if (currentState == newState)
        return;

    Serial.printf("State change: %s -> %s\n",
                  stateToString(currentState),
                  stateToString(newState));

    currentState = newState;
}