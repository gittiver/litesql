set(EXECUTABLES example)

set(example_SOURCES example.cpp exampledatabase.cpp exampledatabase.hpp exampledatabase.xml auth.xml exampledatabase.dot)

# variables defined by findSQLite3.cmake
#  SQLITE3_INCLUDE_DIRS - where to find sqlite3.h
#  SQLITE3_LIBRARIES - the libraries to link against
#  SQLITE3_DEFINITIONS - switches required for SQLite3

set(example_LIBS litesql litesql-util litesql_sqlite ${SQLITE3_LIBRARIES})

# add mysql -libs
if(WITH_MYSQL)
	list(APPEND example_LIBS litesql_mysql ${MYSQL_LIBRARIES})
endif(WITH_MYSQL)

# add postgresql -libs
if(WITH_POSTGRES)
	list(APPEND example_LIBS litesql_postgresql ${POSTGRESQL_LIBRARIES})
endif(WITH_POSTGRES)

# get the directory of the database xml definition
get_source_file_property(WFILE exampledatabase.xml LOCATION)
get_filename_component(WDIR ${WFILE} PATH)

add_custom_command(
   OUTPUT ${WDIR}/exampledatabase.cpp ${WDIR}/exampledatabase.hpp 
   COMMAND litesql-gen -v -t c++ exampledatabase.xml
   MAIN_DEPENDENCY exampledatabase.xml
   WORKING_DIRECTORY ${WDIR}
   DEPENDS litesql-gen exampledatabase.xml auth.xml
   )

add_custom_command(
   OUTPUT ${WDIR}/exampledatabase.dot
   COMMAND litesql-gen -t graphviz exampledatabase.xml
   MAIN_DEPENDENCY exampledatabase.xml
   WORKING_DIRECTORY ${WDIR}
   DEPENDS litesql-gen exampledatabase.xml 
   )

 
add_executable(example ${example_SOURCES} ${example_HEADERS})

target_link_libraries (example ${example_LIBS})

set_property(TARGET example APPEND PROPERTY COMPILE_DEFINITIONS ${SQLITE3_DEFINITIONS})

if (WIN32 AND MSVC)
	set_property(TARGET example APPEND PROPERTY COMPILE_DEFINITIONS YY_NEVER_INTERACTIVE YY_NO_UNISTD_H)
endif (WIN32 AND MSVC)

install(TARGETS example RUNTIME DESTINATION bin COMPONENT examples)

include_directories(../library ${SQLITE3_INCLUDE_DIRS})

