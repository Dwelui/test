CC := gcc

AR := ar
ARFLAGS := rcs

# Source --------------------------------
TARGET := build/libtest.a

CFLAGS := -Wall -Wextra -Wpedantic -std=c23 -Iinclude

SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/obj/%.o)


# Tests ---------------------------------
TEST_RUNNER := build/tests/run-tests

TEST_CFLAGS := $(CFLAGS) -g -fsanitize=address -O0
TEST_LDFLAGS := -fsanitize=address

TEST_SRC := $(wildcard tests/*.c)
TEST_OBJ := $(TEST_SRC:tests/%.c=build/tests/obj/%.o)

.PHONY: all test clean compdb

# Source --------------------------------
all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^

build/obj/%.o: src/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@


# Tests ---------------------------------
build/tests/obj/%.o: tests/%.c
	@mkdir -p $(@D)
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

$(TEST_RUNNER): $(OBJ) $(TEST_OBJ)
	@mkdir -p $(@D)
	@$(CC) $^ $(TEST_LDFLAGS) -o $@

test: $(TEST_RUNNER)
	@$(TEST_RUNNER)


# Tools ---------------------------------
clean:
	rm -rf build

compdb:
	bear --output compile_commands.json -- $(MAKE) clean $(TEST_RUNNER)
