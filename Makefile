CC=gcc
CFLAGS=-I.
CXXFLAGS+= -Wall -g
DEPS=

SOURCES=config.cpp main.cpp signals.cpp tokenizer.cpp log.cpp process.cpp util.cpp exceptions.cpp
#OBJECTS=$(SOURCES:.c=.o)
OBJECTS=$(SOURCES:.cpp=.o)
NOMAIN=$(SOURCES:main.cpp=)
TESTS=$(NOMAIN:signals.cpp=) test_main.cpp test_config.cpp test_log.cpp test_process.cpp test_util.cpp # test_tokenizer.cpp
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
	# https://wiki.wxwidgets.org/Valgrind_Suppression_File_Howto
	valgrind --show-leak-kinds=all --leak-check=full --show-reachable=yes --error-limit=no --suppressions=./valgrind.supp ./test -s
