#pragma once
#include "GPIO.hpp"
#include "Common.hpp"
#include <cstdint>
#include <mutex>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

class DHT11 {
    GPIO& gpio_;
    std::mutex& timing_mutex_;
    static constexpr int PIN = 20;
    bool wait_while(bool level, int timeout_us) {
        auto start = Clock::now();
        while (gpio_.read(PIN) == level) {
            if (Clock::now() - start > std::chrono::microseconds(timeout_us)) return false;
        }
        return true;
    }
    bool once(double& temperature, double& humidity) {
        std::lock_guard<std::mutex> lock(timing_mutex_);
        uint8_t bytes[5]{};
        gpio_.write(PIN, false);
        gpio_.mode(PIN, true);
        std::this_thread::sleep_for(20ms);
        gpio_.write(PIN, true);
        auto until = Clock::now() + 30us;
        while (Clock::now() < until) {}
        gpio_.mode(PIN, false);
        if (!wait_while(true, 120) || !wait_while(false, 120) || !wait_while(true, 120)) return false;
        for (int i = 0; i < 40; ++i) {
            if (!wait_while(false, 110)) return false;
            auto start = Clock::now();
            if (!wait_while(true, 120)) return false;
            auto width = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - start).count();
            bytes[i / 8] = static_cast<uint8_t>((bytes[i / 8] << 1) | (width > 50 ? 1 : 0));
        }
        if (static_cast<uint8_t>(bytes[0] + bytes[1] + bytes[2] + bytes[3]) != bytes[4]) return false;
        humidity = bytes[0] + bytes[1] / 10.0;
        temperature = bytes[2] + bytes[3] / 10.0;
        return true;
    }
public:
    DHT11(GPIO& g, std::mutex& m) : gpio_(g), timing_mutex_(m) { gpio_.mode(PIN, false); }
    bool read(double& temperature, double& humidity) {
        for (int i = 0; i < 5 && !stop_requested; ++i) {
            if (once(temperature, humidity)) return true;
            if (i < 4) std::this_thread::sleep_for(1100ms);
        }
        return false;
    }
};
