MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libepdf.la

EPDFHEADERS = \
Epdf.h \
epdf_document.h \
epdf_enum.h \
epdf_page.h \
epdf_index.h \
epdf_main.h \
epdf_forward.h

PRIVATEHEADERS = epdf_private.h

installed_headersdir = $(prefix)/include/epdf
installed_headers_DATA = $(EPDFHEADERS)

libepdf_la_SOURCES = \
epdf_document.c \
epdf_page.c \
epdf_index.c \
epdf_main.c

libepdf_la_CPPFLAGS = @EPDF_CFLAGS@
libepdf_la_LIBADD = @EPDF_LIBS@
libepdf_la_LDFLAGS = -no-undefined -version-info @version_info@

EXTRA_DIST = $(EPDFHEADERS) $(PRIVATEHEADERS)
