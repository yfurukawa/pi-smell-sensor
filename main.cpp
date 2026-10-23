#include "SmellMonitor.hpp"
#include "Common.hpp"
#include <csignal>
#include <iostream>
#include <exception>

volatile sig_atomic_t stop_requested = 0;
extern "C" void on_signal(int) { stop_requested = 1; }

int main() {
    struct sigaction sa{};
    sa.sa_handler = on_signal;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);
    try { SmellMonitor monitor; monitor.run(); }
    catch (const std::exception& ex) { std::cerr << "FATAL: " << ex.what() << '\n'; return 1; }
    return 0;
}
