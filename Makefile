CFLAGS := -Wall 
DEBUG_FLAGS := -g -DDEBUG

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

TARGET := clox
DEBUG_TARGET := clox_debug

# Default target
all: $(TARGET)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(DEBUG_TARGET)

$(TARGET): $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o $@

$(DEBUG_TARGET): $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o $@

%: %.c
	gcc $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(DEBUG_TARGET)

.PHONY: all debug clean