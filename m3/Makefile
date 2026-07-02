CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

TARGET := m3
SOURCES := main.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.obj
