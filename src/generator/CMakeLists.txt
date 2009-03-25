add_executable(litesql-gen
					args.cpp
					common.cpp
					filestream.cpp filestream.hpp
					litesql-gen-cpp.cpp litesql-gen-cpp.hpp 
					litesql-gen-python.cpp litesql-gen-python.hpp 
					litesql-gen-main.cpp
					cppgenerator.hpp 
					litesql-gen-graphviz.hpp 
					litesql-gen-graphviz.cpp
)

target_link_libraries (litesql-gen litesql xmlreader)

IF (WIN32)
	set_property(TARGET litesql-gen PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS YY_NEVER_INTERACTIVE YY_NO_UNISTD_H)
ENDIF (WIN32)

install(TARGETS litesql-gen RUNTIME DESTINATION bin COMPONENT generator)

INCLUDE_DIRECTORIES(../library ../libxmlreader)
