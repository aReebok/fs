CC = gcc
# Added -Iinclude to CFLAGS so the compiler knows where to find the .h files
# Added --coverage flag for code coverage analysis
CFLAGS = -Wall -O2 -Iinclude --coverage

# LDFLAGS is explicitly defined for the linker, also with --coverage
LDFLAGS = --coverage

# Directories
BUILD_DIR = build
SRC_DIR = src
TEST_DIR = test/build

SRC_FILES = bufcache.c buffer.c cdllist.c diskdrv.c util.c talloc.c 

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
	$(CC) $(LDFLAGS) $(BUILD_OBJS) -o $(TARGET)

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

# #-------------------------------------------------------------------------------
# # LCOV Coverage Rules
# #-------------------------------------------------------------------------------

REPORTS_DIR = $(TEST_DIR)/reports
coverage: test
	@echo "Generating code coverage report..."
	mkdir -p $(REPORTS_DIR)
	lcov -q --capture --initial --directory . --output-file $(REPORTS_DIR)/coverage.info --ignore-errors inconsistent,inconsistent,inconsistent,inconsistent,unsupported,unsupported,unsupported,unsupported,format,format
	lcov -q --capture --directory . --output-file $(REPORTS_DIR)/coverage.info --ignore-errors inconsistent,inconsistent,inconsistent,inconsistent,unsupported,unsupported,unsupported,unsupported,format,format
	lcov -q --remove $(REPORTS_DIR)/coverage.info '*/test/*' '*/include/*' --output-file $(REPORTS_DIR)/filtered_coverage.info
	genhtml -q $(REPORTS_DIR)/filtered_coverage.info --output-directory $(TEST_DIR)/coverage_report
	@echo "Code coverage report generated in ./coverage_report/index.html"
	@echo "==============================================================="
	@echo "--- File Coverage Summary ---"
	lcov --list $(REPORTS_DIR)/filtered_coverage.info
