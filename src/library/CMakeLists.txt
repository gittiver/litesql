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
ENDIF (WITH_SQLITE3)

IF (WITH_MYSQL)
  add_library(  ${LIBNAME}_mysql
        				mysql.cpp
   
  )
ENDIF (WITH_MYSQL)

IF (WITH_POSTGRES)
  add_library(  ${LIBNAME}_postgresql
				postgresql.cpp 
  )
ENDIF (WITH_POSTGRES)

IF (WITH_ODBC)
  add_library(  ${LIBNAME}_odbc
      				odbc_backend.cpp 
		      		odbc_backend.hpp 
  )
ENDIF (WITH_ODBC)

INCLUDE_DIRECTORIES(../../src/library)

