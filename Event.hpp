#pragma once
#include <chrono>
struct Event {
    std::chrono::system_clock::time_point timestamp;
    bool switch_high;
    int smell;
};
