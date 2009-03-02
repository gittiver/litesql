add_executable(litesql-gen
					litesql-gen.cpp 
					litesql-gen-actions.cpp 
					xmlobjects.cpp 
					md5.cpp 
					litesql-gen-cpp.cpp 
					litesql-gen-cpp.hpp 
					litesql-gen-main.cpp 
					litesql-gen-main.hpp 
					xmlobjects.hpp 
					md5.hpp 
					litesql-gen.hpp 
					cppgenerator.hpp 
					xmlobjects.hpp 
					litesql-gen-graphviz.hpp 
					litesql-gen-graphviz.cpp
)

target_link_libraries (litesql-gen litesql)

IF (WIN32)
	set_property(TARGET litesql-gen PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS YY_NEVER_INTERACTIVE YY_NO_UNISTD_H)
ENDIF (WIN32)


INCLUDE_DIRECTORIES(../../include ../../win32 ../library)
