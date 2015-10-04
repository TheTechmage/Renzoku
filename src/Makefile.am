#CFLAGS = --pedantic -Wall -std=c99 -O2
#CXXFLAGS = -Wall -g -Isrc/ -static -lyaml-cpp -std=c++11
AM_CXXFLAGS = --pedantic -Wall -O2 -g -std=c++11
AM_LDFLAGS = -Bstatic -lyaml-cpp -Bdynamic

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
noinst_LIBRARIES = librenzoku.a
librenzoku_adir = $(pkglibdir)/@PACKAGE@
#librenzoku_a_HEADERS = \
#											 exceptions.hpp \
#											 log.hpp \
#											 process.hpp \
#											 signals.hpp \
#											 util.hpp \
#											 watcher.hpp
librenzoku_a_SOURCES = \
											 config.cpp \
											 config.hpp \
											 config_sections.hpp \
											 exceptions.cpp \
											 exceptions.hpp \
											 i_configfile.hpp \
											 log.cpp \
											 log.hpp \
											 process.cpp \
											 process.hpp \
											 procman.cpp \
											 procman.hpp \
											 signals.cpp \
											 signals.hpp \
											 util.cpp \
											 util.hpp \
											 watcher.cpp \
											 watcher.hpp
#											 tokenizer.cpp
#											 tokenizer.hpp


bin_PROGRAMS = renzoku
renzokudir = $(pkglibdir)/@PACKAGE@
renzoku_LDADD = librenzoku.a
renzoku_SOURCES = \
									main.hpp \
									main.cpp
