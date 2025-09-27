CC = gcc

# Production flags
CFLAGS = -Wall -O2 -Iinclude
LDFLAGS = 

# Coverage flags (used only for coverage target)
CFLAGS_COVERAGE = -Wall -O0 -g -Iinclude --coverage
LDFLAGS_COVERAGE = --coverage

# Directories
BUILD_DIR = build
SRC_DIR = src
TEST_DIR = test/build
REPORTS_DIR = $(TEST_DIR)/reports

SRC_FILES = bufcache.c buffer.c cdllist.c diskdrv.c util.c talloc.c bfs_drv.c
OBJS_SRC = $(patsubst %.c, %.o, $(SRC_FILES))
OBJS = main.o $(OBJS_SRC)
BUILD_OBJS = $(addprefix $(BUILD_DIR)/,$(OBJS))
TARGET = $(BUILD_DIR)/main

#-------------------------------------------------------------------------------
# Build / Run Targets
#-------------------------------------------------------------------------------
all: $(BUILD_DIR) $(TARGET) run

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(BUILD_OBJS)
	$(CC) $(LDFLAGS) $(BUILD_OBJS) -o $(TARGET)

$(BUILD_DIR)/main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_DIR)

test: all
	# Delete old coverage data
	rm -f $(TEST_DIR)/*.gcda $(TEST_DIR)/*.gcno
	@echo "Building and running tests..."
	@make -C test

#-------------------------------------------------------------------------------
# Coverage Target
#-------------------------------------------------------------------------------
coverage: clean
	@echo "Building and running tests for coverage..."
	mkdir -p $(BUILD_DIR) $(REPORTS_DIR)
	$(MAKE) CFLAGS="$(CFLAGS_COVERAGE)" LDFLAGS="$(LDFLAGS_COVERAGE)" all
	@make test
	lcov -q --capture --initial --directory $(BUILD_DIR) \
		--output-file $(REPORTS_DIR)/coverage.info
# 		--ignore-errors unsupported,unsupported
	lcov -q --capture --directory $(BUILD_DIR) \
		--output-file $(REPORTS_DIR)/coverage.info
# 		--ignore-errors unsupported,unsupported
	lcov -q --remove $(REPORTS_DIR)/coverage.info '*/include/*' \
		--output-file $(REPORTS_DIR)/filtered_coverage.info \
		--ignore-errors unused
	genhtml -q $(REPORTS_DIR)/filtered_coverage.info --output-directory $(TEST_DIR)/coverage_report
	@echo "Coverage report generated at $(TEST_DIR)/coverage_report/index.html"
	lcov --list $(REPORTS_DIR)/filtered_coverage.info
