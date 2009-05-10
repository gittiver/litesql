#MESSAGE (parentdir=${PARENT_DIRECTORY})
#add_custom_command(
#   OUTPUT exampledatabase.cpp exampledatabase.hpp 
#   COMMAND echo parentdir=${PARENT_DIRECTORY}
#   WORKING_DIRECTORY ${PARENT_DIRECTORY}
#   DEPENDS litesql-gen  
#   )

#add_custom_command(
#   OUTPUT exampledatabase.cpp exampledatabase.hpp 
#   COMMAND litesql-gen -t exampledatabase.xml
#   WORKING_DIRECTORY ${PARENT_DIRECTORY}
#   DEPENDS litesql-gen  
#   )

add_executable(example example.cpp exampledatabase.cpp exampledatabase.hpp exampledatabase.xml)

target_link_libraries (example litesql_sqlite litesql)

IF (WIN32)
	set_property(TARGET example PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS YY_NEVER_INTERACTIVE YY_NO_UNISTD_H)
ENDIF (WIN32)


INCLUDE_DIRECTORIES(../../include ../../win32 ../library)
