# Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
# Date        : Tuesday Sep 14, 2021 15:28:56 NPT
# License     : MIT

DEBUG=0

STD=-std=c99
WARN=-Wall -Wextra

OPTIMIZE=-O2
DEBUG_FLAGS=-g

CFLAGS=$(STD) -MMD

ifeq ($(DEBUG), 1)
	CFLAGS += $(DEBUG_FLAGS) $(WARN) -DDEBUG
else
	CFLAGS += $(OPTIMIZE) -DNDEBUG
endif

BUILD_DIR=./build
SRC_DIR=./src

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS=$(patsubst %.o, %.d, $(OBJS))

TARGET=path_mod

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ 

-include ($(DEPS))

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)/*



