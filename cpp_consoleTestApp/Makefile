# Simple Makefile
# command: make
# command: make clean

csrc := $(wildcard *.c)
cppsrc := $(wildcard *.cpp)
obj := $(csrc:%.c=%.o) $(cppsrc:%.cpp=%.o)
hdr := $(wildcard *.h) $(wildcard *.hpp)
app := cpp_consoleTestApp

CC = gcc
CFLAGS = -std=c17
CXX = g++
CXXFLAGS = -std=c++17 
FLAGS = -Wall -g
LIBS = -lm
LDFLAGS = -ggdb

# compiler flags:
#  -g     adds debugging information to the executable file
#  -ggdb  adds GNU Debugger support to the executable file
#  -Wall  turns on most, but not all, compiler warnings
#  -lm    adds math library functions to linker
#  -c     flag to generate the object file
#  -o     flag to put the output of the compilation in the file named on the left side of the :

%.o:: %.c $(hdr)
	$(CC) -c -o $@ $< $(CFLAGS) $(FLAGS)

%.o:: %.cpp $(hdr)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(FLAGS)

$(app): $(obj)
	$(CXX) -o $(app) $(obj) $(LDFLAGS) $(LIBS)

all: $(app)

.PHONY: clean

clean:
	rm -f $(app) $(obj)
