if DEBUG
AM_CFLAGS = -g3 -O0 -std=c++11
AM_CXXFLAGS = -g3 -O0 -std=c++11
else
AM_CFLAGS = -O2 -std=c++11
AM_CXXFLAGS = -O2 -std=c++11
endif
noinst_LIBRARIES = libparser.a
libparser_a_SOURCES = \
											main.cpp \
											lexicon.cpp \
											lexicon.hpp \
											tokenizer.cpp \
											tokenizer.hpp \
											parser.cpp \
											parser.hpp

bin_PROGRAMS = parser
parserdir = $(pkglibdir)/@PACKAGE@
parser_LDADD = libparser.a
parser_SOURCES = \
								 main.cpp

