CC = gcc
INCLUDES = src/*.h
INCLUDES += src/*.c
CFLAGS=-Wall -g

all: out
run: out

out:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@

clean:
	rm -f *.o out
