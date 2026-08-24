CC := gcc

AR := ar
ARFLAGS := rcs

TARGET := build/libtest.a

CFLAGS := -Wall -Wextra -Wpedantic -std=c23
CFLAGS += -Iinclude

SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/obj/%.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^

build/obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build
