CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
DIRS     := ui business/model business/control business/exception \
            business/validation business/repository infra
LIB_SRCS := $(wildcard $(addsuffix /*.cpp,$(DIRS)))
HDRS     := $(wildcard $(addsuffix /*.h,$(DIRS)))
SRCS     := main.cpp $(LIB_SRCS)
TARGET   := rpg

$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean
