set(libmodel_SOURCES 
			generator.cpp generator.hpp
			xmlgenerator.cpp 
			cppgenerator.cpp 
			logger.cpp logger.hpp
			xmlobjects.cpp 
			md5.cpp 
			litesql-gen-cpp.cpp 
			litesql-gen-cpp.hpp 
			xmlobjects.hpp 
			md5.hpp 
			cppgenerator.hpp
			xmlobjects.hpp 
			litesql-gen-graphviz.hpp 
			litesql-gen-graphviz.cpp
	)
set(litesql-gen_SOURCES 
			litesql-gen-main.cpp 
			litesql-gen-main.hpp 
	)

# for new parser
if (WITH_SAX_PARSER)
   list(APPEND libmodel_SOURCES xmlparser.cpp
                                xmlparser.hpp
                                objectmodel.cpp
                                objectmodel.hpp 
)
else (WITH_SAX_PARSER)
   list(APPEND libmodel_SOURCES 
          litesql-gen-actions.cpp 
          litesql-gen.cpp 
          litesql-gen.hpp)
endif(WITH_SAX_PARSER)


add_library(model ${libmodel_SOURCES}) 
add_executable(litesql-gen ${litesql-gen_SOURCES})

if (MSVC)
  add_subdirectory(win32-ui)
  set_property(TARGET model APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS YY_NEVER_INTERACTIVE YY_NO_UNISTD_H ${EXPAT_DEFINITIONS})
  set_property(TARGET model APPEND PROPERTY LINK_FLAGS_DEBUG)
  set_property(TARGET litesql-gen APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS YY_NEVER_INTERACTIVE YY_NO_UNISTD_H ${EXPAT_DEFINITIONS})
  set_property(TARGET litesql-gen APPEND PROPERTY LINK_FLAGS_DEBUG)
endif(MSVC)


if (WITH_SAX_PARSER)
  target_link_libraries (litesql-gen litesql model ${EXPAT_LIBRARIES})
else(WITH_SAX_PARSER)
  target_link_libraries (litesql-gen litesql model ${EXPAT_LIBRARIES})
endif(WITH_SAX_PARSER)

install(TARGETS litesql-gen RUNTIME DESTINATION bin COMPONENT generator)
INCLUDE_DIRECTORIES(../library ${EXPAT_INCLUDE_DIRS})
