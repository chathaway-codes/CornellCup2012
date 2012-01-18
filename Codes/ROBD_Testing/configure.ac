dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(ROBD_Testing, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_PROG_CXX

PKG_CHECK_MODULES(QT, [QtCore >= 4.6])
AC_SUBST(QT_CFLAGS)
AC_SUBST(QT_LIBS)

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

