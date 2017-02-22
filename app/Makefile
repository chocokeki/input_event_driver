#CC := arm-linux-gnueabi-gcc
CC = gcc
OBJS = read_input_event.o
SRCS = $(OBJS:.o=.c)
CFLAGS = -Wall -g
TARGET = read_event

all: $(TARGET)

read_event: $(OBJS)
	@echo -e '\n\nCompile Start...'
	$(CC) $(CFLAGS) -o $@ $(OBJS)
	@cp -f read_event ../bin/

clean:
	@echo "Cleaning...."
	rm -f $(OBJS) $(TARGET)
	rm -f ../bin/$(TARGET)

dep:
	gccmakedep $(SRCS)

clena: clean
celan: clean
celna: clean

