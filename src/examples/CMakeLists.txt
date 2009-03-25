# get the directory of the database xml definition
get_source_file_property(WFILE exampledatabase.xml LOCATION)
get_filename_component(WDIR ${WFILE} PATH)

add_custom_command(
   OUTPUT ${WDIR}/exampledatabase.cpp ${WDIR}/exampledatabase.hpp 
   COMMAND litesql-gen -t c++ exampledatabase.xml
   MAIN_DEPENDENCY exampledatabase.xml
   WORKING_DIRECTORY ${WDIR}
   DEPENDS litesql-gen exampledatabase.xml 
   )

add_custom_command(
   OUTPUT ${WDIR}/exampledatabase.dot
   COMMAND litesql-gen -t graphviz exampledatabase.xml
   MAIN_DEPENDENCY exampledatabase.xml
   WORKING_DIRECTORY ${WDIR}
   DEPENDS litesql-gen exampledatabase.xml 
   )

 
add_executable(example example.cpp exampledatabase.cpp exampledatabase.hpp exampledatabase.xml exampledatabase.dot)

target_link_libraries (example litesql_sqlite litesql)

IF (WIN32)
	set_property(TARGET example APPEND PROPERTY COMPILE_DEFINITIONS YY_NEVER_INTERACTIVE YY_NO_UNISTD_H)
ENDIF (WIN32)

install(TARGETS example RUNTIME DESTINATION bin COMPONENT examples)

INCLUDE_DIRECTORIES(../library)
