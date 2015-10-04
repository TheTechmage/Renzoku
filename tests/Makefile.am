AM_CXXFLAGS = --pedantic -Wall -I. -I../src/ -g -std=c++11
AM_LDFLAGS = -lyaml-cpp
check_PROGRAMS = test_runner
test_runner_SOURCES = \
test_config.cpp \
test_log.cpp \
test_main.cpp \
test_process.cpp \
test_util.cpp
#test_tokenizer.cpp \
test_runner_LDADD = ../src/librenzoku.a

TESTS = test_runner
