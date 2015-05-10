#the compilerr: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -g -Wall -std=c++0x 

INCLUDES = -I/home/pi/BCM2835 -I/home/pi/WilkieRobot

SRCS = /home/pi/BCM2835/BCM2835.cpp WilkieRobot.cpp CMPS11.cpp

LIBRARY = -lncurses -lpthread
# the build target executable:
TARGET = WilkieRobot

all: $(TARGET)
	
$(TARGET):$(TARGET).cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) $(LIBRARY)
	
clean:
	$(RM) $(TARGET)
