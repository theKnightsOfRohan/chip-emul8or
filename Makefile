# Configuration
LOG_FILE ?= log.txt
TARGET = chip8
ROM ?= ./test/roms/programs/Random\ Number\ Test\ \[Matthew\ Mikolay,\ 2010\].ch8
CC = gcc
CFLAGS = -g -c
# EXTRA_FLAGS ?= -Werror -Wall -Wextra -fsanitize=undefined,address

# Directories
SRC_DIR = src
TEST_DIR = tests/test
OBJ_DIR = obj
BIN_DIR = bin
LOG_DIR = log
DEBUG_DIR = debug

# Source files
SRC = $(wildcard $(SRC_DIR)/*.c)
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)

# Object files
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/test/%.o) \
           $(TEST_SRC:$(TEST_DIR)/%.c=$(OBJ_DIR)/test/%.o)

# Compiler flags
ALL_CFLAGS = $(CFLAGS) $(EXTRA_FLAGS)

# Main targets
.PHONY: all
all: build run

# Create directories
.PHONY: dirs
dirs:
	mkdir -p $(OBJ_DIR) $(BIN_DIR) $(LOG_DIR) $(DEBUG_DIR)

# Pattern rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c dirs
	$(CC) $(ALL_CFLAGS) -o $@ $< -DLOG_FILE_NAME='"$(LOG_FILE)"'

$(OBJ_DIR)/test/%.o: $(SRC_DIR)/%.c dirs
	if [ "$*" != "main" ]; then \
		$(CC) $(ALL_CFLAGS) -o $@ $< -DLOG_FILE_NAME='"test_log.txt"'; \
	fi

$(OBJ_DIR)/test/%.o: $(TEST_DIR)/%.c dirs
	$(CC) $(ALL_CFLAGS) -o $@ $< -DLOG_FILE_NAME='"test_log.txt"'

# Link rules
$(BIN_DIR)/$(TARGET): $(OBJ)
	$(CC) -o $@ $^

$(BIN_DIR)/test/$(TARGET): $(TEST_OBJ)
	$(CC) -o $@ $^

.PHONY: build
build: compile link

.PHONY: compile
compile: $(OBJ)

.PHONY: link
link: $(BIN_DIR)/$(TARGET)

.PHONY: run
run: $(BIN_DIR)/$(TARGET)
	./$< $(ROM)

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/* $(LOG_DIR)/*

# Test targets
test: clean build_test run_test

build_test: compile_test link_test

compile_test: $(TEST_OBJ)

link_test: $(BIN_DIR)/test/$(TARGET)

run_test: $(BIN_DIR)/test/$(TARGET)
	./$< $(ROM)

# Debug targets
.PHONY: debug debug_test
debug:
	make build

	if [ ! -f $(DEBUG_DIR)/setup.lldb ]; then \
		mkdir -p $(DEBUG_DIR); \
		touch $(DEBUG_DIR)/setup.lldb; \
	fi

	lldb ./$(BIN_DIR)/$(TARGET) $(ROM)

debug_test:
	make build_test

	if [ ! -f $(DEBUG_DIR)/test/setup.lldb ]; then \
		mkdir -p $(DEBUG_DIR)/test; \
		touch $(DEBUG_DIR)/test/setup.lldb; \
	fi

	lldb -S $(DEBUG_DIR)/test/setup.lldb -- ./$(BIN_DIR)/test/$(TARGET) $(ROM)
