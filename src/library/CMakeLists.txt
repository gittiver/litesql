set(LIBNAME litesql)

##############################################################################
# litesql-util library
##############################################################################
set(litesql_util_SOURCES string.cpp ../../include/litesql/string.hpp)    

add_library(litesql-util STATIC ${litesql_util_SOURCES})
set_property(TARGET litesql-util PROPERTY POSITION_INDEPENDENT_CODE ON)

install(TARGETS litesql-util EXPORT litesql-util
            RUNTIME DESTINATION lib
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static 
            COMPONENT devel)
install(EXPORT litesql-util DESTINATION lib)
if (MSVC)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/debug/litesql-utild.lib DESTINATION lib/static
    COMPONENT devel)
endif(MSVC)

##############################################################################
# litesql library
##############################################################################
set(litesql_SOURCES    database.cpp 
            expr.cpp 
            selectquery.cpp 
            persistent.cpp 
            backend.cpp 
            updatequery.cpp 
            field.cpp 
            datasource.cpp 
            compatibility.hpp 
            compatibility.cpp 
            datetime.cpp
            ../../include/litesql/commontypes.h
            ../../include/litesql/database.hpp 
            ../../include/litesql/expr.hpp 
            ../../include/litesql/selectquery.hpp 
            ../../include/litesql/persistent.hpp 
            ../../include/litesql/backend.hpp 
            ../../include/litesql/updatequery.hpp 
../../include/litesql/field.hpp
../../include/litesql/cursor.hpp
            ../../include/litesql/datasource.hpp
            ../../include/litesql/datetime.hpp
	    plugin.hpp plugin.cpp
	    sharedlibrary.hpp sharedlibrary.cpp
)

add_library(  ${LIBNAME} STATIC ${litesql_SOURCES})
set_property(TARGET ${LIBNAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
install(TARGETS ${LIBNAME} EXPORT ${LIBNAME}
            RUNTIME DESTINATION lib 
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static 
            COMPONENT devel)
install(EXPORT ${LIBNAME} DESTINATION lib)

if (MSVC)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/debug/litesqld.lib DESTINATION lib/static
    COMPONENT devel)
endif(MSVC)


if (LITESQL_WITH_SQLITE3)
  set(${LIBNAME}_sqlite_SRC
                sqlite3/sqlite3_backend.cpp 
                sqlite3/sqlite3.hpp )
add_library(${LIBNAME}_sqlite STATIC ${${LIBNAME}_sqlite_SRC})
	      
if(LITESQL_USE_SYSTEM_SQLITE AND SQLITE3_FOUND)
    message("use sqlite3 from system: ${SQLITE3_LIBRARIES}")
    list(APPEND BACKEND_INCLUDES ${SQLITE3_INCLUDE_DIR})
    list(APPEND BACKEND_LIBRARIES ${SQLITE3_LIBRARIES})
    list(APPEND BACKEND_DEFINITIONS ${SQLITE3_DEFINITIONS})
endif(LITESQL_USE_SYSTEM_SQLITE AND SQLITE3_FOUND)

if (NOT SQLITE3_FOUND)
  ExternalProject_Add(libsqlite3
    GIT_REPOSITORY https://github.com/gittiver/sqlite3-amalgamation.git
	GIT_TAG v3_29_0
    PREFIX 3rdparty
	CMAKE_ARGS -D SQLITE3_USE_SHARED_CRT:bool=OFF
    INSTALL_COMMAND ""
  )
  if (MSVC)
    add_library(sqlite3 STATIC IMPORTED)
    set_property(TARGET sqlite3 PROPERTY IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/src/libsqlite3-build/Debug/sqlite3.lib)
    set_property(TARGET sqlite3 PROPERTY IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/src/libsqlite3-build/Release/sqlite3.lib)
    set_property(TARGET sqlite3 PROPERTY IMPORTED_LOCATION_MINSIZEREL ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/src/libsqlite3-build/MinSizeRel/sqlite3.lib)
    add_dependencies(sqlite3 libsqlite3)
	set(SQLITE3_INCLUDE_DIR  ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/src/libsqlite3-build/3rdparty/src/sqlite3-src)
    include_directories(${SQLITE3_INCLUDE_DIR})
  endif(MSVC)
  message("use internal sqlite3")
  set (SQLITE3_LIBRARIES sqlite3)
  message("- use sqlite3 from: ${SQLITE3_LIBRARIES}")
 endif(NOT SQLITE3_FOUND)


install(TARGETS ${LIBNAME}_sqlite  EXPORT ${LIBNAME}_sqlite
            RUNTIME DESTINATION lib 
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static 
            COMPONENT devel)
install(EXPORT ${LIBNAME}_sqlite DESTINATION lib)
if (MSVC)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/debug/${LIBNAME}_sqlited.lib DESTINATION lib/static COMPONENT devel)
endif(MSVC)

###########################
# backend as shared library
###########################
add_library(${LIBNAME}_backend_sqlite3 SHARED ${${LIBNAME}_sqlite_SRC}
sqlite3/sqlite3_backend_plugin.cpp
)
target_link_libraries(${LIBNAME}_backend_sqlite3 PRIVATE litesql PRIVATE litesql-util)
if(LITESQL_USE_SYSTEM_SQLITE AND SQLITE3_FOUND)
target_link_libraries(${LIBNAME}_backend_sqlite3 PRIVATE ${SQLITE3_LIBRARIES})
else(LITESQL_USE_SYSTEM_SQLITE AND SQLITE3_FOUND)
target_link_libraries(${LIBNAME}_backend_sqlite3 PRIVATE sqlite3)
endif(LITESQL_USE_SYSTEM_SQLITE AND SQLITE3_FOUND)

endif(LITESQL_WITH_SQLITE3)

if (LITESQL_WITH_MYSQL)
  add_library(  ${LIBNAME}_mysql SHARED
                        mysql/mysql.cpp
   
  )
  install(TARGETS ${LIBNAME}_mysql  EXPORT ${LIBNAME}_mysql 
            RUNTIME DESTINATION lib 
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static 
            COMPONENT devel)
  install(EXPORT ${LIBNAME}_mysql DESTINATION lib)
if (MSVC)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/debug/${LIBNAME}_mysqld.lib DESTINATION lib/static COMPONENT devel)
endif(MSVC)

endif (LITESQL_WITH_MYSQL)

if (LITESQL_WITH_OCILIB)
  add_library(  ${LIBNAME}_ocilib SHARED
                        ocilib_backend.cpp
                        ocilib_backend.hpp
   
  )
  install(TARGETS ${LIBNAME}_ocilib  EXPORT ${LIBNAME}_ocilib 
            RUNTIME DESTINATION lib 
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static 
            COMPONENT devel)
  install(EXPORT ${LIBNAME}_ocilib DESTINATION lib)
if (MSVC)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/debug/${LIBNAME}_ocilibd.lib DESTINATION lib/static COMPONENT devel)
endif(MSVC)

endif (LITESQL_WITH_OCILIB)

if (LITESQL_WITH_POSTGRES)
  add_library(  ${LIBNAME}_postgresql SHARED
                postgresql.cpp 
  )
  install(TARGETS ${LIBNAME}_postgresql EXPORT ${LIBNAME}_postgresql  
            RUNTIME DESTINATION lib 
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static 
            COMPONENT devel)
if (MSVC)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/debug/${LIBNAME}_postgresqld.lib DESTINATION lib/static COMPONENT devel)
endif(MSVC)
endif (LITESQL_WITH_POSTGRES)

if (LITESQL_WITH_ODBC)
  add_library(  ${LIBNAME}_odbc SHARED
                      odbc_backend.cpp 
                      odbc_backend.hpp 
  )
  install(TARGETS ${LIBNAME}_odbc EXPORT ${LIBNAME}_odbc 
            RUNTIME DESTINATION lib 
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static 
            COMPONENT devel)
if (MSVC)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/debug/${LIBNAME}_odbcd.lib DESTINATION lib/static COMPONENT devel)
endif(MSVC)

endif (LITESQL_WITH_ODBC)

include_directories(../../src/library ../../include )

