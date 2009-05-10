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
				sqlite3_backend.cpp 
				sqlite3.c
				sqlite3.hpp
				sqlite3.h
				sqlite3ext.h
)
set_property(TARGET ${LIBNAME}_sqlite PROPERTY COMPILE_DEFINITIONS HAVE_LIBSQLITE3)
set_property(TARGET ${LIBNAME} APPEND PROPERTY COMPILE_DEFINITIONS HAVE_LIBSQLITE3)
    target_link_libraries(${LIBNAME} ${LIBNAME}_sqlite)
    if (NOT WIN32)
        target_link_libraries(${LIBNAME}_sqlite dl pthread)
    endif(NOT WIN32)

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

IF (WITH_ODBC)
    message("ASFASEF")
add_library(  ${LIBNAME}_odbc
				odbc_backend.cpp 
				odbc_backend.hpp 
)
set_property(TARGET ${LIBNAME}_odbc PROPERTY COMPILE_DEFINITIONS HAVE_ODBC)
set_property(TARGET ${LIBNAME} APPEND PROPERTY COMPILE_DEFINITIONS HAVE_ODBC)
ENDIF (WITH_ODBC)

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

   IF (WITH_ODBC)
   set_property(TARGET ${LIBNAME}_odbc APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS HAVE_ODBC)
   ENDIF (WITH_ODBC)
	
ENDIF (WIN32)

INCLUDE_DIRECTORIES(../../include ../../win32 ../../src/library)

