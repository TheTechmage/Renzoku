#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(renzoku, 0.1.0, renzoku@frostyfrog.net)
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_LANG(C++)

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h sys/time.h unistd.h])
# http://lists.gnu.org/archive/html/autoconf/2003-11/msg00155.html
#AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([yaml-cpp/yaml.h boost/shared_ptr.hpp])
#AC_LANG_POP([C++])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_CHECK_FUNCS([getcwd gettimeofday memset select strerror])

# Static Library
AC_PROG_RANLIB

#AC_CONFIG_FILES([Makefile])
AC_OUTPUT(Makefile src/Makefile doc/Makefile examples/Makefile man/Makefile
					tests/Makefile)
AC_OUTPUT
