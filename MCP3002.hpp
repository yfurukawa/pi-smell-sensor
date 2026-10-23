#pragma once
#include <cstdint>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

using namespace std::chrono_literals;

class MCP3002 {
    int fd_ = -1;
public:
    MCP3002() {
        fd_ = ::open("/dev/spidev0.0", O_RDWR);
        if (fd_ < 0) throw std::runtime_error("open /dev/spidev0.0 failed; enable SPI");
        uint8_t mode = SPI_MODE_0, bits = 8;
        uint32_t speed = 500000;
        if (ioctl(fd_, SPI_IOC_WR_MODE, &mode) < 0 ||
            ioctl(fd_, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0 ||
            ioctl(fd_, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
            throw std::runtime_error("SPI configuration failed");
    }
    ~MCP3002() { if (fd_ >= 0) ::close(fd_); }
    MCP3002(const MCP3002&) = delete;
    MCP3002& operator=(const MCP3002&) = delete;
    int read(int channel) {
        if (channel != 0 && channel != 1) throw std::invalid_argument("ADC channel");
        // MCP3002 2-byte command: start, single-ended, channel, MSBF.
        uint8_t tx[2] = {static_cast<uint8_t>(0x68 | (channel << 4)), 0};
        uint8_t rx[2] = {0, 0};
        spi_ioc_transfer x{};
        x.tx_buf = reinterpret_cast<uintptr_t>(tx);
        x.rx_buf = reinterpret_cast<uintptr_t>(rx);
        x.len = 2; x.speed_hz = 500000; x.bits_per_word = 8;
        if (ioctl(fd_, SPI_IOC_MESSAGE(1), &x) < 0) return -1;
        return ((rx[0] & 3) << 8) | rx[1];
    }
};
