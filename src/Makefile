CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
DIRS     := ui business/model business/control business/exception \
            business/validation business/repository infra
LIB_SRCS := $(wildcard $(addsuffix /*.cpp,$(DIRS)))
HDRS     := $(wildcard $(addsuffix /*.h,$(DIRS)))
SRCS     := main.cpp $(LIB_SRCS)
TARGET   := rpg
TEST_DIR := ../test
TEST_BIN := $(TEST_DIR)/test_runner
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)

$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

run: $(TARGET)
	./$(TARGET)

$(TEST_BIN): $(TEST_SRCS) $(LIB_SRCS) $(HDRS) $(wildcard $(TEST_DIR)/*.h)
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) $(LIB_SRCS) -o $@

test: $(TEST_BIN)
	$(TEST_BIN)

clean:
	rm -f $(TARGET) $(TEST_BIN)

.PHONY: run test clean
