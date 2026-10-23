#pragma once
#include "GPIO.hpp"
#include "MCP3002.hpp"
#include "TGS8100.hpp"
#include "DHT11.hpp"
#include "CSVLogger.hpp"
#include "Event.hpp"
#include "Common.hpp"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

class SmellMonitor {
    GPIO gpio_;
    MCP3002 adc_;
    std::mutex timing_mutex_;
    TGS8100 smell_{gpio_, adc_, timing_mutex_};
    DHT11 dht_{gpio_, timing_mutex_};
    CSVLogger logger_{"/var/log/smell.log"};
    std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    std::deque<Event> queue_;
    std::atomic<bool> active_{true};
    std::thread sampling_, switch_, measurement_;
    static constexpr size_t MAX_QUEUE = 64;
    void enqueue(bool switch_high) {
        Event event{std::chrono::system_clock::now(), switch_high, smell_.latest()};
        std::lock_guard<std::mutex> lock(queue_mutex_);
        if (queue_.size() == MAX_QUEUE) { std::cerr << "WARNING: measurement queue full\n"; return; }
        queue_.push_back(event);
        queue_cv_.notify_one();
    }
    void sample_loop() {
        auto next = Clock::now();
        while (active_) {
            try { smell_.sample(); }
            catch (const std::exception& ex) { std::cerr << "ADC: " << ex.what() << '\n'; }
            next += 1s;
            if (next < Clock::now()) next = Clock::now();
            std::this_thread::sleep_until(next);
        }
    }
    void switch_loop() {
        bool previous = gpio_.read(21);
        while (active_) {
            bool current = gpio_.read(21);
            if (previous && !current) enqueue(false);
            previous = current;
            std::this_thread::sleep_for(1ms);
        }
    }
    void measurement_loop() {
        while (true) {
            Event event;
            {
                std::unique_lock<std::mutex> lock(queue_mutex_);
                queue_cv_.wait(lock, [&] { return !queue_.empty() || !active_; });
                if (queue_.empty() && !active_) return;
                event = queue_.front(); queue_.pop_front();
            }
            double t = 0, h = 0;
            bool valid = dht_.read(t, h);
            try { logger_.write(event, valid, t, h); }
            catch (const std::exception& ex) { std::cerr << "LOG: " << ex.what() << '\n'; }
        }
    }
public:
    SmellMonitor() {
        gpio_.mode(21, false);
        gpio_.pullup(21);
    }
    void run() {
        sampling_ = std::thread(&SmellMonitor::sample_loop, this);
        switch_ = std::thread(&SmellMonitor::switch_loop, this);
        measurement_ = std::thread(&SmellMonitor::measurement_loop, this);
        std::cerr << "Started; 20 minute warm-up (switch events accepted)\n";
        auto warmup_end = Clock::now() + 20min;
        while (!stop_requested && Clock::now() < warmup_end) std::this_thread::sleep_for(100ms);
        if (!stop_requested) {
            enqueue(gpio_.read(21));
            auto next = Clock::now() + 5min;
            while (!stop_requested) {
                if (Clock::now() >= next) {
                    enqueue(gpio_.read(21));
                    next += 5min;
                    if (next < Clock::now()) next = Clock::now() + 5min;
                }
                std::this_thread::sleep_for(50ms);
            }
        }
        active_ = false;
        queue_cv_.notify_all();
        if (switch_.joinable()) switch_.join();
        if (sampling_.joinable()) sampling_.join();
        if (measurement_.joinable()) measurement_.join();
        std::cerr << "Stopped\n";
    }
};
