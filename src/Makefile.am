#CFLAGS = --pedantic -Wall -std=c99 -O2
#CXXFLAGS = -Wall -g -Isrc/ -static -lyaml-cpp -std=c++11
AM_CXXFLAGS = --pedantic -Wall -O2 -std=c++11
AM_LDFLAGS = -Bstatic -lyaml-cpp -Bdynamic

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
noinst_LIBRARIES = librenzoku.a
librenzoku_adir = $(pkglibdir)/@PACKAGE@
librenzoku_a_HEADERS = \
config.hpp \
config_sections.hpp \
exceptions.hpp \
i_configfile.hpp \
log.hpp \
process.hpp \
signals.hpp \
tokenizer.hpp \
util.hpp \
watcher.hpp
librenzoku_a_SOURCES = \
config.cpp \
exceptions.cpp \
log.cpp \
process.cpp \
signals.cpp \
tokenizer.cpp \
util.cpp \
watcher.cpp


bin_PROGRAMS = renzoku
renzokudir = $(pkglibdir)/@PACKAGE@
renzoku_HEADERS = main.hpp
renzoku_SOURCES = main.cpp
renzoku_LDADD = librenzoku.a
