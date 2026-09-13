#include "../include/process.h"

std::string stateToString(State s) {
    switch (s) {
        case State::NEW: return "NEW";
        case State::READY: return "READY";
        case State::RUNNING: return "RUNNING";
        case State::TERMINATED: return "TERMINATED";
    }
    return "UNKNOWN";
}
