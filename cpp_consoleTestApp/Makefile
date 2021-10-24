# Simple Makefile
# command: make
# command: make clean

csrc := $(wildcard *.c)
cppsrc := $(wildcard *.cpp)
obj := $(csrc:%.c=%.o) $(cppsrc:%.cpp=%.o)
hdr := $(wildcard *.h) $(wildcard *.hpp)
app := cpp_consoleTestApp

CC = g++
CFLAGS = -std=c++17 -lm -g

# compiler flags:
#  -g     adds debugging information to the executable file
#  -ggdb  adds GNU Debugger support to the executable file
#  -Wall  turns on most, but not all, compiler warnings
#  -lm    adds math functions to linker
#  -c     flag to generate the object file
#  -o     flag to put the output of the compilation in the file named on the left side of the :

%.o:: %.c $(hdr)
	$(CC) -ggdb -Wall -c -o $@ $< $(CFLAGS)

%.o:: %.cpp $(hdr)
	$(CC) -ggdb -Wall -c -o $@ $< $(CFLAGS)

$(app): $(obj)
	$(CC) -ggdb -Wall -o $(app) $(obj) $(CFLAGS)

all: $(app)

.PHONY: clean

clean:
	rm -f $(app) $(obj)
