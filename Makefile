CC = gcc
INCLUDES = *.h
INCLUDES += *.c
CFLAGS=-Wall -g

all: out
run: out

out:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@

clean:
	rm -f *.o out
