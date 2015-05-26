CC=gcc
CFLAGS=-I.
DEPS=

SOURCES=config.cpp main.cpp signals.cpp tokenizer.cpp log.cpp process.cpp
#OBJECTS=$(SOURCES:.c=.o)
OBJECTS=$(SOURCES:.cpp=.o)
NOMAIN=$(SOURCES:main.cpp=)
TESTS=$(NOMAIN:signals.cpp=) test_tokenizer.cpp test_config.cpp test_log.cpp test_process.cpp
TEST_OBJECTS=$(TESTS:.cpp=.o)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
%.o: %.cpp $(DEPS)
	$(CXX) -c -std=c++11 -o $@ $< $(CXXFLAGS)

.PHONY: all clean test testv

all: builder

builder: $(OBJECTS)
	$(CXX) $(OBJECTS) -o builder

clean:
	rm *.o builder test

build_test: $(TEST_OBJECTS)
	$(CXX) $(TEST_OBJECTS) -o test

test: build_test
	./test
testv: build_test
	valgrind ./test -s
