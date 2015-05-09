
CC=gcc
CFLAGS=-I.
DEPS=

SOURCES=watcher.cpp config.cpp main.c program.cpp
OBJECTS=$(SOURCES:.c=.o)
OBJECTS=$(OBJECTS:.cpp=.o)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY: all clean

all: builder

builder: watcher.o
	$(CXX) watcher.o -o builder

clean:
	rm *.o builder
