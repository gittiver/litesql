AC_INIT(src/litesql.hpp)
AM_INIT_AUTOMAKE(litesql,0.3)
AM_CONFIG_HEADER(src/config.h)
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([inttypes.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_CHECK_FUNCS([gettimeofday memset strchr strdup strrchr strstr strtol])

AC_CHECK_LIB([sqlite3], [sqlite3_open])
AC_CHECK_LIB([pq], [PQconnectdb])
AC_CHECK_LIB([mysqlclient], [mysql_real_connect])
AH_TEMPLATE(HAVE_POSTGRESQL8, [postgresql 8.0 header is in different location])
AC_CHECK_HEADER([postgresql/8.0/libpq-fe.h], [AC_DEFINE(HAVE_POSTGRESQL8)])

AC_PROG_LIBTOOL
AC_OUTPUT(Makefile src/Makefile) 

