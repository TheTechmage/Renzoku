#CFLAGS = --pedantic -Wall -std=c99 -O2
#CXXFLAGS = -Wall -g -Isrc/ -static -lyaml-cpp -std=c++11
AM_CXXFLAGS = --pedantic -Wall -O2 -std=c++11
AM_LDFLAGS = -Bstatic -lyaml-cpp -Bdynamic

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
noinst_LIBRARIES = librenzoku.a
librenzoku_a_SOURCES = \
config.cpp \
config.hpp \
config_sections.hpp \
exceptions.cpp \
exceptions.hpp \
i_configfile.hpp \
log.cpp \
log.hpp \
Makefile.am \
process.cpp \
process.hpp \
signals.cpp \
signals.hpp \
tokenizer.cpp \
tokenizer.hpp \
util.cpp \
util.hpp \
watcher.cpp \
watcher.hpp


bin_PROGRAMS = renzoku
renzoku_SOURCES = \
main.cpp \
main.hpp
renzoku_LDADD = librenzoku.a
