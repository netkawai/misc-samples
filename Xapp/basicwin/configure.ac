dnl very old configure.ac (it was configure.in 1999)
dnl Process this file with autoconf to produce a configure script.
AC_INIT
AC_CONFIG_SRCDIR([basicwin.c])	

AM_INIT_AUTOMAKE(basicwin,0.0.1) 

dnl Checks for programs.
AC_PROG_CC	

dnl Checks for libraries.

dnl Checks for header files.
AC_PATH_XTRA 
m4_warn([obsolete],
[The preprocessor macro `STDC_HEADERS' is obsolete.
  Except in unusual embedded environments, you can safely include all
  ISO C90 headers unconditionally.])dnl
# Autoupdate added the next two lines to ensure that your configure
# script's behavior did not change.  They are probably safe to remove.
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

dnl Checks for typedefs, structures, and compiler characteristics.
dnl Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT


