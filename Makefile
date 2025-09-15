CC = gcc
CFLAGS = -Wall -O2
BUILD_DIR = build
TEST_DIR = test/build

OBJS = main.o talloc.o cdllist.o buffer.o bufcache.o diskdrv.o helpercode.o
BUILD_OBJS = $(addprefix $(BUILD_DIR)/,$(OBJS))

TARGET = $(BUILD_DIR)/main

all: $(BUILD_DIR) $(TARGET) run 

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(BUILD_OBJS)
	$(CC) $(CFLAGS) $(BUILD_OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -rf $(TEST_DIR)

test: all 
	@echo "Building and running tests..."
	@make -C test