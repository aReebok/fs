# --- Detect OS for portability ---
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # macOS: Homebrew paths
    BREW_PREFIX := $(shell brew --prefix)
    PLATFORM_CXXFLAGS := -isystem $(BREW_PREFIX)/include
    PLATFORM_LDFLAGS := -L$(BREW_PREFIX)/lib
else
    # Linux (or other OS)
    PLATFORM_CXXFLAGS :=
    PLATFORM_LDFLAGS :=
endif

CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -I$(PROJECT_ROOT)/include $(PLATFORM_CXXFLAGS)
LDFLAGS = -lgtest -lpthread

OBJS = test_talloc.o test_main.o test_bufcache.o
TEST_OBJS = $(addprefix $(BUILD_DIR)/,$(OBJS))
BUILD_OBJS = ../build/talloc.o ../build/bufcache.o ../build/buffer.o ../build/cdllist.o ../build/util.o ../build/diskdrv.o

PROJECT_ROOT = ..
BUILD_DIR = $(PROJECT_ROOT)/test/build
TARGET = $(BUILD_DIR)/main

.PHONY: all run clean

all: $(BUILD_DIR) $(TARGET) run

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c++
	$(CXX) $(CXXFLAGS) -c $< -o $@ -c

$(TARGET): $(TEST_OBJS) $(BUILD_OBJS)
	$(CXX) $(TEST_OBJS) $(BUILD_OBJS) -o $(TARGET) $(LDFLAGS) $(PLATFORM_LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
