dnl Copyright © 2010,2012,2015 Saleem Abdulrasool <compnerd@compnerd.org>
dnl vim: set sw=4 sts=4 et fdm=marker fmr={{{,}}} :

dnl {{{ program, version
AC_PREREQ([2.59])
AC_INIT([jsonxx], [0.0.1], [compnerd@compnerd.org])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER([config/config.h])
AM_INIT_AUTOMAKE([1.11.1 subdir-objects dist-xz])
dnl }}}

dnl {{{ basic toolchain checks
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
AC_PROG_INSTALL

LT_INIT
dnl }}}

dnl {{{ library checks
dnl }}}

dnl {{{ output
AM_SILENT_RULES([yes])
AC_OUTPUT([Makefile])
dnl }}}

