AC_PREREQ([2.57])
AC_INIT([emupdf], [0.1], [lunohod@openinkpot.org])

AM_INIT_AUTOMAKE(foreign)

AC_PROG_CC_C99
AC_PROG_LIBTOOL

INTERFACE_CURRENT="0"
INTERFACE_REVISION="1"
INTERFACE_AGE="0"
version_info=${INTERFACE_CURRENT}:${INTERFACE_REVISION}:${INTERFACE_AGE}
AC_SUBST(version_info)

requirements_emupdf="evas ecore"

PKG_CHECK_MODULES(EPDF, [evas ecore mupdf])
PKG_CHECK_MODULES(ECORE_EVAS, [ecore-evas])

AC_SUBST(requirements_emupdf)

AC_OUTPUT([
Makefile
epdf.pc
src/Makefile
src/lib/Makefile
])
