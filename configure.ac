#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libini], [0.1.0], [zguangyu123@gmail.com])
AC_CONFIG_SRCDIR([iniparser.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror 1.11])

AM_PROG_AR

LT_PREREQ([2.2.4])
LT_INIT([dlopen])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strchr strdup strerror])

PKG_CHECK_MODULES([CHECK], [check])

AC_CONFIG_FILES([Makefile
                 tests/Makefile])
AC_OUTPUT
