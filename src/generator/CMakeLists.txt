set(litesql-gen_SOURCES 
			xmlobjects.cpp 
			md5.cpp 
			litesql-gen-cpp.cpp 
			litesql-gen-cpp.hpp 
			litesql-gen-main.cpp 
			litesql-gen-main.hpp 
			xmlobjects.hpp 
			md5.hpp 
			cppgenerator.hpp 
			xmlobjects.hpp 
			litesql-gen-graphviz.hpp 
			litesql-gen-graphviz.cpp
)

# for new parser
if (WITH_SAX_PARSER)
   list(APPEND litesql-gen_SOURCES xmlparser.cpp
                                   xmlparser.hpp
                                   objectmodel.cpp
                                   objectmodel.hpp 
)
else (WITH_SAX_PARSER)
   list(APPEND litesql-gen_SOURCES 
          litesql-gen-actions.cpp 
          litesql-gen.cpp 
          litesql-gen.hpp)
endif(WITH_SAX_PARSER)

add_executable(litesql-gen ${litesql-gen_SOURCES})

if (WITH_SAX_PARSER)
  target_link_libraries (litesql-gen litesql ${LIBXML2_LIBRARIES})
else(WITH_SAX_PARSER)
  target_link_libraries (litesql-gen litesql)
endif(WITH_SAX_PARSER)

if (MSVC)
  set_property(TARGET litesql-gen APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS YY_NEVER_INTERACTIVE YY_NO_UNISTD_H)
  set_property(TARGET litesql-gen APPEND PROPERTY LINK_FLAGS_DEBUG /NODEFAULTLIB:MSVCRT )
endif(MSVC)

install(TARGETS litesql-gen RUNTIME DESTINATION bin COMPONENT generator)

INCLUDE_DIRECTORIES(../library ${LIBXML2_INCLUDE_DIR})
