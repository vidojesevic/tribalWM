# Tribal Window Manager

# Compilers flags
CC = gcc
CFLAGS = -g -Wall -Wextra -std=c2x

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
TWM = tribalwm

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Test files
TEST_DIR = tests/unit
TEST_SRC_FILES = $(wildcard $(TEST_DIR)/test_*.c)
TEST_OBJ_FILES := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRC_FILES))

.PHONY: all

# Targets
all: $(BUILD_DIR)/$(TWM)

$(BUILD_DIR)/$(TWM): $(OBJ_FILES) $(BUILD_DIR)/main.o
	$(CC) $(CFLAGS) $^ -o $@ -lX11

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -I$(INCLUDE_DIR) $< -o $@

$(BUILD_DIR)/main.o: main.c
	$(CC) $(CFLAGS) -c -I$(INCLUDE_DIR) $< -o $@

.PHONY: test
test: $(TEST_OBJ_FILES) $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ -lcriterion
	./test --verbose

$(BUILD_DIR)/test_%.o: $(TEST_DIR)/test_%.c
	$(CC) $(CFLAGS) -c -I$(INCLUDE_DIR) $< -o $@

.PHONY: clean
clean:
	rm -vf $(BUILD_DIR)/*.o test
