#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(renzoku, 0.1.0, renzoku@frostyfrog.net)
AC_CONFIG_AUX_DIR([build-aux])
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
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([yaml-cpp/yaml.h boost/shared_ptr.hpp])

# debug? daemon mode?
AC_ARG_ENABLE(recursion,
							[AS_HELP_STRING([--disable-recursion],
															[Disable recursive directory searching])],
							[feature_recursion=no], #replace with ac_define
							[feature_recursion=yes])
AC_ARG_ENABLE(log-level,
							[AS_HELP_STRING([--enable-log-level[=level]],
															[Specify the logging level:\
															 debug: The most verbose level\
															 info: A bit less verbose\
															 warn: Warnings only\
															 error: Errors only])],
															[ if test "x$enableval" = "xdebug" ; then
															 AC_DEFINE([LOGLEVEL], [debug],
																				 [Logging set to debug level])
															 elif test "x$enableval" = "xinfo" ; then
															 AC_DEFINE([LOGLEVEL], [info],
																				 [Logging set to info level])
															 elif test "x$enableval" = "xwarn" ; then
															 AC_DEFINE([LOGLEVEL], [warn],
																				 [Logging set to warning level])
															 elif test "x$enableval" = "xerror" ; then
															 AC_DEFINE([LOGLEVEL], [error],
																				 [Logging set to error level])
															 else
																 AC_MSG_FAILURE(
																								[Invalid log level]
																								)
															 fi
															 ])

AC_ARG_WITH([yaml],
						[AS_HELP_STRING([--without-yaml],
														[Disable yaml config support])],
						[],
						[with_yaml=yes])
LIBYAMLCPP=
AS_IF([test "x$with_yaml" != xno],
			[AC_CHECK_LIB([yaml-cpp], [main],
										[AC_SUBST([LIBYAMLCPP], ["-lyaml-cpp"])
										AC_DEFINE([HAVE_YAML], [1],
															[Define if you have libyaml-cpp])
										],
										[AC_MSG_FAILURE(
										 [yaml-cpp test failed (--without-yaml to disable)]
										)],
										[]
										)])
AC_ARG_ENABLE(debug,
							AS_HELP_STRING([--enable-debug],
								[build with additional debugging code]),
							[case "${enableval}" in
								yes) debug=true; AC_DEFINE([__DEBUG__]) ;;
								no) debug=false ;;
								*)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug])
								;; esac],
							[debug=false])
AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")
AC_LANG_POP([C++])

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

AC_REQUIRE_AUX_FILE([tap-driver.sh])
AC_PROG_AWK

#AC_CONFIG_FILES([Makefile])
AC_OUTPUT(Makefile src/Makefile doc/Makefile examples/Makefile man/Makefile
					tests/Makefile src/parser/Makefile)
AC_OUTPUT
