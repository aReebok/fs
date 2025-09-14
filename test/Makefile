CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -I$(PROJECT_ROOT)
LDFLAGS = -lgtest -lpthread

OBJS = test_talloc.o test_main.o test_bufcache.o
TEST_OBJS = $(addprefix $(BUILD_DIR)/,$(OBJS))
BUILD_OBJS = ../build/talloc.o ../build/bufcache.o ../build/buffer.o ../build/cdllist.o ../build/helpercode.o

BUILD_DIR = build
TARGET = $(BUILD_DIR)/main

all: $(BUILD_DIR) $(TARGET) run

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c++
	$(CXX) $(CXXFLAGS) -c $< -o $@ -c 

$(TARGET): $(TEST_OBJS) $(BUILD_OBJS)
	$(CXX) $(TEST_OBJS) $(BUILD_OBJS) -o $(TARGET) $(LDFLAGS) 

run: $(TARGET) 
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)