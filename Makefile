CC = gcc
CFLAGS = -Wall -O2
BUILD_DIR = build

OBJS = main.o talloc.o cdllist.o buffer.o
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
