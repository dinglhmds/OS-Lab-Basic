CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c99 -O2 -I../common -D_POSIX_C_SOURCE=200809L
TARGET  := filesystem

SRCS    := fs.c disk.c bitmap.c main.c ../common/utils.c
OBJS    := $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
