CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC = \
	src/main.cpp \
	src/scheduler.cpp \
	src/process.cpp \
	src/memory/paging.cpp

TARGET = minios

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean