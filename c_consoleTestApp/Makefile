src := $(wildcard *.c)
obj := $(src:%.c=%.o)
hdr := $(wildcard *.h)
app := c_consoleTestApp

CC=gcc
CFLAGS = -std=c17 -lm

# compiler flags:
#  -g    adds debugging information to the executable file
#  -ggdb adds GNU Debugger support to the executable file
#  -Wall turns on most, but not all, compiler warnings
#  -lm   adds math functions to linker

%.o: %.c $(hdr)
	$(CC) -ggdb -Wall -c -o $@ $< $(CFLAGS)

$(app): $(obj)
	$(CC) -ggdb -Wall -o $(app) $(obj) $(CFLAGS)

all: $(app)

.PHONY: clean

clean:
	rm -f $(app) $(obj)
