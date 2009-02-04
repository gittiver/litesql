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
				sqlite3.cpp 
				postgresql.cpp 
				mysql.cpp 
				compatibility.cpp 
				datetime.cpp
				sqlite3_l.c
)

IF (WIN32)
	set_property(TARGET ${LIBNAME} PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS HAVE_LIBSQLITE3)
	set_property(TARGET ${LIBNAME} PROPERTY SUFFIX .lib)
ENDIF (WIN32)

INCLUDE_DIRECTORIES(../../include ../../win32)
