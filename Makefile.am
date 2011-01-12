# vim: set ft=make ts=8 :

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

INCLUDES = -I src

lib_LTLIBRARIES = src/libjsonxx.la

src_libjsonxx_la_CXXFLAGS =
src_libjsonxx_la_LIBADD =
src_libjsonxx_la_SOURCES = src/jsonxx.cc \
			   src/jsonxx.hh \
			   $(NULL)

MAINTAINERCLEANFILES = aclocal.m4 configure Makefile.in

