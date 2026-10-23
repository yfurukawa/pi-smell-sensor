#pragma once
#include "Event.hpp"
#include <string>
#include <fstream>
#include <ctime>
#include <stdexcept>
#include <utility>

using namespace std::chrono_literals;

class CSVLogger {
    std::string path_;
public:
    explicit CSVLogger(std::string path) : path_(std::move(path)) {
        std::ifstream input(path_, std::ios::binary | std::ios::ate);
        bool empty = !input || input.tellg() == std::streampos(0);
        if (empty) {
            std::ofstream out(path_, std::ios::app);
            if (!out) throw std::runtime_error("Cannot open CSV log");
            out << "date,time,temperature,humidity,smell,switch\n";
        }
    }
    void write(const Event& e, bool valid, double t, double h) {
        std::time_t when = std::chrono::system_clock::to_time_t(e.timestamp);
        std::tm tm{};
        localtime_r(&when, &tm);
        char date[16], clock[16];
        std::strftime(date, sizeof(date), "%Y-%m-%d", &tm);
        std::strftime(clock, sizeof(clock), "%H:%M:%S", &tm);
        std::ofstream out(path_, std::ios::app);
        if (!out) throw std::runtime_error("CSV append failed");
        out << date << ',' << clock << ',';
        if (valid) { out.setf(std::ios::fixed); out.precision(1); out << t << ',' << h; }
        else out << ',';
        out << ',';
        if (e.smell >= 0) out << e.smell;
        out << ',' << (e.switch_high ? "high" : "low") << '\n';
        if (!out) throw std::runtime_error("CSV write failed");
    }
};
