#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([libmidisd], [0.0.1], [bsorahan@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror gnu])
AC_CONFIG_SRCDIR([src/midisd_receive.c])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h strings.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strcasecmp strdup strtoul])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
