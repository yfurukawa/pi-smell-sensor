#pragma once
#include <chrono>
#include <csignal>
using Clock = std::chrono::steady_clock;
extern volatile sig_atomic_t stop_requested;
