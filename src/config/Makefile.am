#CFLAGS = --pedantic -Wall -std=c99 -O2
#CXXFLAGS = -Wall -g -Isrc/ -static -lyaml-cpp -std=c++11
AM_CXXFLAGS = --pedantic -Wall -O2 -g -std=c++11
AM_LDFLAGS = -lconfig++

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
noinst_LIBRARIES = librenconf.a
librenconf_adir = $(pkglibdir)/@PACKAGE@
librenconf_a_SOURCES = \
											 config.cpp \
											 config.hpp
#											 tokenizer.cpp
#											 tokenizer.hpp


bin_PROGRAMS = renconf
renconfdir = $(pkglibdir)/@PACKAGE@
renconf_LDADD = librenconf.a
renconf_SOURCES = \
									main.cpp
