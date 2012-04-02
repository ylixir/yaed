#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([yaed], [0.2], [ylixir@gmail.com], [yaed], [http://code.google.com/p/yaed])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
#AC_CONFIG_SRCDIR([tab-contents.h])
AM_CONFIG_HEADER(config.h)

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99

# Checks for libraries.
PKG_CHECK_MODULES(PACKAGE, gtk+-3.0 gtksourceview-3.0)
# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.

AC_CONFIG_FILES(Makefile)

AC_OUTPUT
