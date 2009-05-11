set(litesql-gen_SOURCES litesql-gen.cpp 
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

# for new parser					
#         xmlparser.cpp xmlparser.hpp
#			objectmodel.cpp objectmodel.hpp
#)

add_executable(litesql-gen ${litesql-gen_SOURCES})

target_link_libraries (litesql-gen litesql)
# for new parser ${LIBXML2_LIBRARIES}

if (MSVC)
   set_property(TARGET litesql-gen APPEND 
                  PROPERTY LINK_FLAGS /NODEFAULTLIB:MSVCRT 
	               PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS YY_NEVER_INTERACTIVE YY_NO_UNISTD_H)
endif(MSVC)

install(TARGETS litesql-gen RUNTIME DESTINATION bin COMPONENT generator)

INCLUDE_DIRECTORIES(../library ${LIBXML2_INCLUDE_DIR})
