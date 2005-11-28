AC_PREREQ(2.50)
AC_INIT(src/library/database.cpp)
AM_INIT_AUTOMAKE(litesql,0.3)
AM_CONFIG_HEADER(config.h)
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

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

AC_CHECK_FUNCS([gettimeofday memset strchr strdup strrchr strstr strtol strtof strtoll atof localtime_r mktime])

# Check for SQLite3.
find_sqlite3="yes"
sqlite3_master_dir=/usr
SQLITE3_CXXFLAGS=""
SQLITE3_LDFLAGS=""
AC_ARG_WITH([sqlite3], AS_HELP_STRING([--with-sqlite3@<:@=PATH@:>@],
[compile with SQLite3 support @<:@default=yes@:>@]),[
case "$withval" in
    yes) ;;
    no) find_sqlite3="no" ;;
    *) sqlite3_master_dir=$withval ;;
esac])
if test "x${find_sqlite3}" = "xyes"
then
    AC_MSG_CHECKING([for SQLite3 header file])
    sqlite3_header="${sqlite3_master_dir}/include/sqlite3.h"
    if test -r "${sqlite3_header}"
    then
	if test "x/usr" != "x${sqlite3_master_dir}"
	then
	    SQLITE3_CXXFLAGS="-I${sqlite3_master_dir}/include"
	    SQLITE3_LDFLAGS="-L${sqlite3_master_dir}/lib"
	fi
    else
	sqlite3_header="not found"
    fi
    AC_MSG_RESULT([${sqlite3_header}])
    if test "${sqlite3_header}" != "not found"
    then
	saved_LDFLAGS="$LDFLAGS"
	LDFLAGS="$LDFLAGS ${SQLITE3_LDFLAGS}"
	AC_CHECK_LIB([sqlite3], [sqlite3_open])
	LDFLAGS="${saved_LDFLAGS}"
    fi
fi
AC_SUBST(SQLITE3_CXXFLAGS)
AC_SUBST(SQLITE3_LDFLAGS)

# Check for MySQL client lib.
AH_TEMPLATE(HAVE_LIBMYSQLCLIENT, [Define to 1 if you have the `mysqlclient' library (-lmysqlclient).])
find_mysql="yes"
mysql_config_path="/usr"
AC_ARG_VAR([MYSQL_CONFIG], [location of mysql_config])
MYSQL_CXXFLAGS=""
MYSQL_LIBS=""
AC_ARG_WITH([mysql], AS_HELP_STRING([--with-mysql@<:@=PATH@:>@],
[compile with MySQL support @<:@default=yes@:>@]),[
case "$withval" in
    yes) ;;
    no) find_mysql="no" ;;
    *) mysql_config_path=$withval ;;
esac])
if test -d "${mysql_config_path}"
then
    mysql_config_path="${mysql_config_path}/bin"
fi
if test "x${find_mysql}" = "xyes"
then
AC_CHECK_PROG([MYSQL_CONFIG], [mysql_config], [${mysql_config_path}/mysql_config], 
	      [not found], [${mysql_config_path}])
if test "x${MYSQL_CONFIG}" != "xnot found"
then
    MYSQL_CXXFLAGS=`${MYSQL_CONFIG} --include`
    MYSQL_LIBS=`${MYSQL_CONFIG} --libs`
    AC_DEFINE([HAVE_LIBMYSQLCLIENT])
fi
fi
AC_SUBST(MYSQL_CXXFLAGS)
AC_SUBST(MYSQL_LIBS)

# Check for PostgreSQL client lib.
find_pgsql="yes"
pgsql_config_path="/usr"
AC_ARG_VAR([PGSQL_CONFIG], [location of pg_config])
PGSQL_CXXFLAGS=""
PGSQL_LDFLAGS=""
AC_ARG_WITH([pgsql], AS_HELP_STRING([--with-pgsql@<:@=PATH@:>@],
[compile with PostgreSQL support @<:@default=yes@:>@]),[
case "$withval" in
    yes) ;;
    no) find_pgsql="no" ;;
    *) pgsql_config_path=$withval ;;
esac])
if test -d "${pgsql_config_path}"
then
    pgsql_config_path="${pgsql_config_path}/bin"
fi
if test "x${find_pgsql}" = "xyes"
then
AC_CHECK_PROG([PGSQL_CONFIG], [pg_config], [${pgsql_config_path}/pg_config], 
	      [not found], [${pgsql_config_path}])
if test "x${PGSQL_CONFIG}" != "xnot found"
then
    PGSQL_CXXFLAGS="-I`${PGSQL_CONFIG} --includedir`"
    PGSQL_LDFLAGS="-L`${PGSQL_CONFIG} --libdir`"
    saved_LDFLAGS="$LDFLAGS"
    LDFLAGS="$LDFLAGS ${PGSQL_LDFLAGS}"
    AC_CHECK_LIB([pq], [PQconnectdb])
    LDFLAGS="${saved_LDFLAGS}"
fi
fi
AC_SUBST(PGSQL_CXXFLAGS)
AC_SUBST(PGSQL_LDFLAGS)

# For now, combine various DB backend flags into master flags.
# Later, it might be useful to only compile and link with the flags when necessary.
CXXFLAGS="$CXXFLAGS ${SQLITE3_CXXFLAGS} ${MYSQL_CXXFLAGS} ${PGSQL_CXXFLAGS}"
LDFLAGS="$LDFLAGS ${SQLITE3_LDFLAGS} ${PGSQL_LDFLAGS}"
LIBS="$LIBS ${MYSQL_LIBS}"

AC_PROG_LIBTOOL
AC_OUTPUT(Makefile include/Makefile include/litesql/Makefile src/Makefile 
          src/library/Makefile src/generator/Makefile src/tests/Makefile src/examples/Makefile) 

