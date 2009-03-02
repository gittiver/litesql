set(LIBNAME litesql)

add_library(  ${LIBNAME}
				database.cpp 
				expr.cpp 
				selectquery.cpp 
				persistent.cpp 
				split.cpp 
				string.cpp 
				backend.cpp 
				updatequery.cpp 
				field.cpp 
				datasource.cpp 
				compatibility.cpp 
				datetime.cpp
)
 
IF (WITH_SQLITE3)
add_library(  ${LIBNAME}_sqlite
				sqlite3.cpp 
				sqlite3_l.c
)
set_property(TARGET ${LIBNAME}_sqlite PROPERTY COMPILE_DEFINITIONS HAVE_LIBSQLITE3)
set_property(TARGET ${LIBNAME} APPEND PROPERTY COMPILE_DEFINITIONS HAVE_LIBSQLITE3 )
ENDIF (WITH_SQLITE3)

IF (WITH_MYSQL)
add_library(  ${LIBNAME}_mysql
				mysql.cpp 
)
set_property(TARGET ${LIBNAME}_mysql PROPERTY COMPILE_DEFINITIONS HAVE_LIBMYSQLCLIENT)
set_property(TARGET ${LIBNAME} APPEND PROPERTY COMPILE_DEFINITIONS HAVE_LIBMYSQLCLIENT)
ENDIF (WITH_MYSQL)

IF (WITH_POSTGRES)
add_library(  ${LIBNAME}_postgresql
				postgresql.cpp 
)
set_property(TARGET ${LIBNAME}_postgresql PROPERTY COMPILE_DEFINITIONS HAVE_LIBPQ)
set_property(TARGET ${LIBNAME} APPEND PROPERTY COMPILE_DEFINITIONS HAVE_LIBPQ)
ENDIF (WITH_POSTGRES)

IF (WIN32)
	set_property(TARGET ${LIBNAME}            APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
   
   IF (WITH_SQLITE3)
	   set_property(TARGET ${LIBNAME}_sqlite     APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS HAVE_LIBSQLITE3)
	ENDIF (WITH_SQLITE3)
	
	IF (WITH_MYSQL)
	set_property(TARGET ${LIBNAME}_mysql      APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS HAVE_LIBSQLITE3)
	ENDIF (WITH_MYSQL)
	
	IF (WITH_POSTGRES)
	set_property(TARGET ${LIBNAME}_postgresql APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS HAVE_LIBSQLITE3)
   ENDIF (WITH_POSTGRES)
	
ENDIF (WIN32)

INCLUDE_DIRECTORIES(../../include ../../win32)

