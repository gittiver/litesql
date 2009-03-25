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

install(TARGETS ${LIBNAME} 
			RUNTIME DESTINATION lib 
			ARCHIVE DESTINATION lib/static 
			COMPONENT devel)

IF (WITH_SQLITE3)
  add_library(  ${LIBNAME}_sqlite
				sqlite3_backend.cpp 
				sqlite3.c
				sqlite3.hpp
				sqlite3.h
				sqlite3ext.h
  ) 
  install(TARGETS ${LIBNAME}_sqlite  
			RUNTIME DESTINATION lib 
			ARCHIVE DESTINATION lib/static 
			COMPONENT devel)
ENDIF (WITH_SQLITE3)

IF (WITH_MYSQL)
  add_library(  ${LIBNAME}_mysql
        				mysql.cpp
   
  )
  install(TARGETS ${LIBNAME}_mysql  
			RUNTIME DESTINATION lib 
			ARCHIVE DESTINATION lib/static 
			COMPONENT devel)
ENDIF (WITH_MYSQL)

IF (WITH_POSTGRES)
  add_library(  ${LIBNAME}_postgresql
				postgresql.cpp 
  )
  install(TARGETS ${LIBNAME}_postgresql  
			RUNTIME DESTINATION lib 
			ARCHIVE DESTINATION lib/static 
			COMPONENT devel)
ENDIF (WITH_POSTGRES)

IF (WITH_ODBC)
  add_library(  ${LIBNAME}_odbc
      				odbc_backend.cpp 
		      		odbc_backend.hpp 
  )
  install(TARGETS ${LIBNAME}_odbc  
			RUNTIME DESTINATION lib 
			ARCHIVE DESTINATION lib/static 
			COMPONENT devel)
ENDIF (WITH_ODBC)

INCLUDE_DIRECTORIES(../../src/library)

