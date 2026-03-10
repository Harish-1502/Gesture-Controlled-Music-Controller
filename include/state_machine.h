#pragma once

enum class State{
    Running,
    Calibrating,
    Idle,
    Sleep
};

State getCurrentState();
void setCurrentState(State newState);