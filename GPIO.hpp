#pragma once
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

using namespace std::chrono_literals;

class GPIO {
    volatile uint32_t* r_ = nullptr;
    static constexpr size_t MAP_SIZE = 4096;
public:
    GPIO() {
        int fd = ::open("/dev/gpiomem", O_RDWR | O_SYNC);
        if (fd < 0) throw std::runtime_error("open /dev/gpiomem failed");
        void* p = mmap(nullptr, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        ::close(fd);
        if (p == MAP_FAILED) throw std::runtime_error("mmap GPIO failed");
        r_ = static_cast<volatile uint32_t*>(p);
    }
    ~GPIO() { if (r_) munmap(const_cast<uint32_t*>(r_), MAP_SIZE); }
    GPIO(const GPIO&) = delete;
    GPIO& operator=(const GPIO&) = delete;
    void mode(int pin, bool output) {
        unsigned reg = pin / 10, shift = (pin % 10) * 3;
        auto v = r_[reg];
        r_[reg] = (v & ~(7u << shift)) | ((output ? 1u : 0u) << shift);
    }
    void write(int pin, bool high) { r_[high ? 7 : 10] = 1u << pin; }
    bool read(int pin) const { return (r_[13] & (1u << pin)) != 0; }
    void pullup(int pin) {
        // BCM2711 (Raspberry Pi 4)
        constexpr unsigned GPPUPPDN0 = 0xE4 / sizeof(uint32_t);

        const unsigned reg = GPPUPPDN0 + pin / 16;
        const unsigned shift = (pin % 16) * 2;

        const uint32_t mask = 3u << shift;

        r_[reg] = (r_[reg] & ~mask) | (1u << shift);
    }
};
