CC := gcc

AR := ar
ARFLAGS := rcs

TARGET := build/libtest.a
TEST_RUNNER := build/tests/run-tests

CFLAGS := -Wall -Wextra -Wpedantic -std=c23 -Iinclude
TEST_CFLAGS := $(CFLAGS) -g -fsanitize=address -O0
TEST_LDFLAGS := -fsanitize=address

SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/obj/%.o)
TEST_SRC := $(wildcard tests/*.c)
TEST_OBJ := $(TEST_SRC:tests/%.c=build/test-obj/%.o)

.PHONY: all test check clean

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^

build/obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

build/test-obj/%.o: tests/%.c
	@mkdir -p $(@D)
	$(CC) $(TEST_CFLAGS) -c $< -o $@

$(TEST_RUNNER): $(OBJ) $(TEST_OBJ)
	@mkdir -p $(@D)
	$(CC) $^ $(TEST_LDFLAGS) -o $@

test: $(TEST_RUNNER)
	@$(TEST_RUNNER)

clean:
	rm -rf build
