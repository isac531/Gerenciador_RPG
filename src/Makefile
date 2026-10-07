CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
SRCS     := $(wildcard *.cpp entity/*.cpp control/*.cpp validation/*.cpp)
TARGET   := rpg

$(TARGET): $(SRCS) $(wildcard *.h entity/*.h control/*.h exception/*.h validation/*.h)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean
