AM_CXXFLAGS = --pedantic -Wall -I. -I../src/ -g -std=c++11 -I../src/parser/
AM_LDFLAGS = -lyaml-cpp
check_PROGRAMS = test_runner
test_runner_SOURCES = \
test_config.cpp \
test_log.cpp \
test_main.cpp \
test_process.cpp \
test_util.cpp
#test_tokenizer.cpp
test_runner_LDADD = ../src/librenzoku.a ../src/parser/libparser.a

TEST_EXTENSIONS = .sh
SH_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
									                  $(top_srcdir)/build-aux/tap-driver.sh
#TESTS = test_runner
TESTS = run_tests.sh test_functional.sh
EXTRA_DIST = $(TESTS)
