#CFLAGS = --pedantic -Wall -std=c99 -O2
#CXXFLAGS = -Wall -g -Isrc/ -static -lyaml-cpp -std=c++11
AM_CXXFLAGS = --pedantic -Wall -O2 -g -std=c++11 -Iparser/ -pthread
AM_LDFLAGS = -Bstatic -Bdynamic
SUBDIRS = parser

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
											 exceptions.cpp \
											 exceptions.hpp \
											 log.cpp \
											 log.hpp \
											 process.cpp \
											 process.hpp \
											 procman.cpp \
											 procman.hpp \
											 threadmgr.cpp \
											 threadmgr.hpp \
											 signals.cpp \
											 signals.hpp \
											 util.cpp \
											 util.hpp \
											 watcher.cpp \
											 watcher.hpp


bin_PROGRAMS = renzoku
renzokudir = $(pkglibdir)/@PACKAGE@
renzoku_LDADD = librenzoku.a parser/libparser.a
renzoku_SOURCES = \
									main.hpp \
									main.cpp
