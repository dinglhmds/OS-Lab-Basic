CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c99 -O2 -I../common -D_POSIX_C_SOURCE=200809L
LDFLAGS := -lpthread
TARGET  := sync

SRCS    := producer_consumer.c readers_writers.c philosophers.c main.c ../common/utils.c
OBJS    := $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
