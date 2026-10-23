#pragma once
#include "GPIO.hpp"
#include "MCP3002.hpp"
#include "Common.hpp"
#include <atomic>
#include <mutex>
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

class TGS8100 {
    GPIO& gpio_;
    MCP3002& adc_;
    std::mutex& timing_mutex_;
    std::atomic<int> latest_{-1};
public:
    TGS8100(GPIO& g, MCP3002& a, std::mutex& m)
        : gpio_(g), adc_(a), timing_mutex_(m) {
        gpio_.write(16, false); gpio_.mode(16, true);
    }
    ~TGS8100() { gpio_.write(16, false); }
    int latest() const { return latest_.load(); }
    int sample() {
        std::lock_guard<std::mutex> lock(timing_mutex_);
        auto start = Clock::now();
        gpio_.write(16, true);
        // Busy wait: do not use sleep_for for sub-millisecond precision.
        while (Clock::now() - start < 1ms) {}
        int value = adc_.read(0);
        gpio_.write(16, false);
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - start).count();
        if (elapsed > 2000) std::cerr << "WARNING: PULSE width " << elapsed << " us (>2000)\n";
        if (value >= 0) latest_.store(value);
        return value;
    }
};
