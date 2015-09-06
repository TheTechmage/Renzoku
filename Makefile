CC=gcc
CFLAGS=-Isrc/ -I.
CXXFLAGS+= -Wall -g -Isrc/ -lyaml-cpp
ifdef COVERAGE
CXXFLAGS+=-fprofile-arcs -ftest-coverage
LDFLAGS+="-lgcov"
endif
DEPS=
BIN_DIR = bin
OBJ_DIR = obj
EXECUTABLE = builder
TEST_EXECUTABLE = TestCode

SOURCES:=config.cpp main.cpp signals.cpp tokenizer.cpp log.cpp process.cpp util.cpp exceptions.cpp watcher.cpp
SOURCES:=$(SOURCES:%.cpp=src/%.cpp)
OBJECTS=$(SOURCES:%.cpp=$(OBJ_DIR)/%.o)
NOMAIN=$(SOURCES:src/main.cpp=)
NOMAIN2=$(NOMAIN:src/watcher.cpp=)
TESTS:=test_main.cpp test_config.cpp test_log.cpp test_process.cpp test_util.cpp # test_tokenizer.cpp
TESTS:=$(NOMAIN2:src/signals.cpp=) $(TESTS:%.cpp=tests/%.cpp)
TEST_OBJECTS=$(TESTS:%.cpp=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: %.c $(DEPS)
	@mkdir -p `dirname $@`
	$(CC) -c -o $@ $< $(CFLAGS)
$(OBJ_DIR)/%.o: %.cpp $(DEPS)
	@mkdir -p `dirname $@`
	$(CXX) -c -std=c++11 -o $@ $< $(CXXFLAGS)

.PHONY: all clean test testv

all: $(BIN_DIR)/$(EXECUTABLE)

$(BIN_DIR)/$(EXECUTABLE): $(OBJECTS)
	@if [[ ! -d $(BIN_DIR) ]]; then mkdir -p $(BIN_DIR); fi
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(OBJECTS) -o $(BIN_DIR)/$(EXECUTABLE)

clean:
	rm -rvf bin/ obj/

build_test: $(TEST_OBJECTS)
	@if [[ ! -d $(BIN_DIR) ]]; then mkdir -p $(BIN_DIR); fi
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(TEST_OBJECTS) -o $(BIN_DIR)/$(TEST_EXECUTABLE)

test: build_test
	$(BIN_DIR)/$(TEST_EXECUTABLE)

testv: build_test
	# https://wiki.wxwidgets.org/Valgrind_Suppression_File_Howto
	if [[ "${TRAVIS}" == "" ]]; then\
		valgrind --show-leak-kinds=all --leak-check=full --show-reachable=yes --error-limit=no --suppressions=./valgrind.supp $(BIN_DIR)/$(TEST_EXECUTABLE) -s;\
	else\
		valgrind --show-reachable=yes --error-limit=no$(BIN_DIR)/$(TEST_EXECUTABLE) -s;\
	fi

coverage: test
	lcov -c -i -d . -o .coverage.base
	lcov -c -d . -o .coverage.run
	lcov -d . -a .coverage.base -a .coverage.run -o .coverage.total
	genhtml --demangle-cpp -s -p `pwd` --no-branch-coverage -o coverage .coverage.total
	rm -f .coverage.base .coverage.run .coverage.total

# vim: set tw=0 :
