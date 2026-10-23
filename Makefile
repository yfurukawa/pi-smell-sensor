CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pthread
TARGET = smell_monitor

all: $(TARGET)

$(TARGET): main.cpp $(wildcard *.hpp)
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
