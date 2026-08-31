CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = deep-fetch
SRCS = main.c system_info.c hardware.c utils.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c deep_fetch.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

uninstall:
	sudo rm /usr/local/bin/deep-fetch

.PHONY: all clean install uninstall
