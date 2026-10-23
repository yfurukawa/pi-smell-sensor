# Smell monitor (C++17)

Raspberry Pi B+ (BCM2835), Raspberry Pi OS 32-bit.

## Build

```sh
sudo apt install build-essential
make
sudo install -m 755 smell_monitor /usr/local/bin/smell_monitor
```

Enable SPI (`sudo raspi-config`) before running.
Run with permission to access `/dev/gpiomem`, `/dev/spidev0.0` and `/var/log/smell.log`.

## Source layout

- `GPIO.hpp`: BCM2835 GPIO memory mapping and I/O
- `MCP3002.hpp`: SPI ADC
- `TGS8100.hpp`: sensor PULSE and ADC sampling
- `DHT11.hpp`: temperature and humidity
- `CSVLogger.hpp`: CSV output
- `Event.hpp`: measurement request
- `SmellMonitor.hpp`: threads, queue, scheduler and switch polling
- `Common.hpp`: clock and signal flag
- `main.cpp`: entry point and signal handling

Each class is in its own header with inline method definitions. This retains the behavior of the previously supplied single-file implementation.

## Cautions

- This is **not** real-time pulse generation: Linux scheduling may extend the 2 ms PULSE limit. Verify with a scope/logic analyzer.
- Verify the MCP3002 SPI command and ADC decode against its datasheet and your wiring before relying on measurements.
- DHT11 bit timing is sensitive to scheduling jitter.
- Switch events are accepted during the 20-minute warm-up.
- CSV: `/var/log/smell.log`.
