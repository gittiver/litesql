if(LITESQL_USE_SYSTEM_EXPAT)
include (${CMAKE_ROOT}/Modules/FindEXPAT.cmake)
mark_as_advanced(EXPAT_INCLUDE_DIRS EXPAT_LIBRARIES EXPAT_FOUND)
endif(LITESQL_USE_SYSTEM_EXPAT)
  
if (NOT EXPAT_FOUND)
  FetchContent_Declare(expat
  		 GIT_REPOSITORY https://github.com/gittiver/libexpat.git
		 GIT_TAG master)
  if(NOT expat_POPULATED)

    set(EXPAT_SHARED_LIBS Off)
    set(EXPAT_BUILD_TOOLS Off)
    set(EXPAT_BUILD_EXAMPLES Off)
    set(EXPAT_BUILD_TESTS Off)
    set(EXPAT_SHARED_LIBS Off)
    set(EXPAT_BUILD_DOCS Off)
    set(EXPAT_BUILD_FUZZERS Off)
    set(EXPAT_WITH_LIBBSD Off)
    set(EXPAT_ENABLE_INSTALL On)
    FetchContent_Populate(expat)

    add_subdirectory(${expat_SOURCE_DIR}/expat ${expat_BINARY_DIR})
  endif()

  set(EXPAT_LIBRARIES expat)
  get_target_property(EXPAT_INCLUDE_DIRS expat INCLUDE_DIRECTORIES)
#  set(EXPAT_INCLUDE_DIRS  ${expat_SOURCE_DIR}/expat/lib ${expat_BIN_DIR})
  
endif (NOT EXPAT_FOUND)

message(EXPAT_LIBRARIES=${EXPAT_LIBRARIES})
message(EXPAT_INCLUDE_DIRS=${EXPAT_INCLUDE_DIRS})
message(EXPAT_DEFINITIONS=${EXPAT_DEFINITIONS})

set(libmodel_SOURCES
			generator.cpp generator.hpp
		 	litesql-gen-xml.cpp litesql-gen-xml.hpp
			litesql-gen-cpp.cpp litesql-gen-cpp.hpp
			xmlobjects.cpp
			md5.cpp
			xmlobjects.hpp
			md5.hpp
			xmlobjects.hpp
			litesql-gen-graphviz.cpp litesql-gen-graphviz.hpp
			litesql-gen-ruby-activerecord.cpp
			litesql-gen-ruby-activerecord.hpp
			litesql-gen-golang.cpp litesql-gen-golang.hpp
			litesql-gen-wxformbuilder.cpp
			litesql-gen-wxformbuilder.hpp
			litesql-gen-protobuf.cpp litesql-gen-protobuf.hpp
	    xmlparser.cpp xmlparser.hpp
            objectmodel.cpp objectmodel.hpp
	)

add_library(model STATIC ${libmodel_SOURCES})
target_include_directories(model PRIVATE ${EXPAT_INCLUDE_DIRS})
target_link_libraries(model PRIVATE ${EXPAT_LIBRARIES})

install(TARGETS model EXPORT model
            RUNTIME DESTINATION lib
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static
            COMPONENT devel)
install(EXPORT model DESTINATION lib)

add_executable(litesql-gen litesql-gen-main.cpp)
add_executable(ddl2litesql ddl2litesql.cpp)

target_link_libraries (litesql-gen litesql litesql-util model)
target_link_libraries (ddl2litesql litesql-util model)

if (LITESQL_WITH_UI)
  add_subdirectory(ui)
endif(LITESQL_WITH_UI)

install(TARGETS litesql-gen RUNTIME DESTINATION bin COMPONENT generator)
