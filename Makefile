CC = gcc
# Added -Iinclude to CFLAGS so the compiler knows where to find the .h files
CFLAGS = -Wall -O2 -Iinclude 

# Directories
BUILD_DIR = build
SRC_DIR = src
TEST_DIR = test/build

SRC_FILES = bufcache.c buffer.c cdllist.c diskdrv.c helpercode.c talloc.c 

# Object file names
# talloc.o is now separated because its source is in SRC_DIR while the others are treated differently
OBJS_SRC = $(patsubst %.c, %.o, $(SRC_FILES))

# All object files for the final link (main.o + all other objects)
OBJS = main.o $(OBJS_SRC)

# Object files to be created in the BUILD_DIR
BUILD_OBJS = $(addprefix $(BUILD_DIR)/,$(OBJS))

TARGET = $(BUILD_DIR)/main

all: $(BUILD_DIR) $(TARGET) run 

# Rule to create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to link the final executable
$(TARGET): $(BUILD_OBJS)
	$(CC) $(CFLAGS) $(BUILD_OBJS) -o $(TARGET)

#-------------------------------------------------------------------------------
# Compilation Rules
#-------------------------------------------------------------------------------

# Rule for compiling main.c (located in the workspace directory)
$(BUILD_DIR)/main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for compiling source files in $(SRC_DIR) (e.g., src/talloc.c -> build/talloc.o)
# The pattern matching takes the object name (e.g., talloc.o) and looks for the source in $(SRC_DIR)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -rf $(TEST_DIR)

test: all 
	@echo "Building and running tests..."
	@make -C test