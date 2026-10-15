CC = g++
RM = rm -f
CP = cp
INCLUDE = -I/usr/incude -I/usr/local/include -I../include
LIB_DIR = -L/usr/lib -L/usr/local/lib -L/usr/lib64 -L/usr/local/lib64
INSTALL_DIR = /usr/local/bin
LIB = -lwiringPi
OPT = -O0 -g3 -Wall -Wnon-virtual-dtor -Woverloaded-virtual -fmessage-length=0
TARGET = pi-fan-controller
SRC= Gpio.cpp DigitalOut.cpp DigitalIn.cpp

all: $(SRC:.cpp=.o) main.o
	$(CC) $(INCLUDE) $(LIB_DIR) $(OPT) -o $(TARGET) $(SRC:.cpp=.o) main.o $(LIB)

$(SRC:.cpp=.o): $(SRC)
	$(CC) $(INCLUDE) $(LIB_DIR) $(OPT) -c $(SRC)

main.o: main.cpp
	$(CC) $(INCLUDE) $(LIB_DIR) $(OPT) -c main.cpp

.PHONY: clean
clean:
	$(RM) *.o $(TARGET) $(TEST_TARGET)

.PHONY: install
install:
	$(CP) $(TARGET) $(INSTALL_DIR)
