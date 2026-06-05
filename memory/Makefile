CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c99 -O2 -I../common -D_POSIX_C_SOURCE=200809L
TARGET  := memory

SRCS    := paging.c partition.c main.c ../common/list.c ../common/utils.c
OBJS    := $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
