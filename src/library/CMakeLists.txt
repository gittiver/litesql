set(LIBNAME litesql)

##############################################################################
# litesql-util library
##############################################################################
set(litesql_util_SOURCES
         string.cpp ../../include/litesql/string.hpp
         logger.cpp ../../include/litesql/logger.hpp
)    

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
        ../../include/litesql/except.hpp
	    plugin.hpp plugin.cpp
	    sharedlibrary.hpp sharedlibrary.cpp
)

add_library(${LIBNAME} STATIC ${litesql_SOURCES})
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


##############################################################################
# litesql sqlite3 library
##############################################################################
if(LITESQL_USE_SYSTEM_SQLITE AND SQLITE3_FOUND)
  message("use sqlite3 from system: ${SQLITE3_LIBRARIES}")
  list(APPEND BACKEND_INCLUDES ${SQLITE3_INCLUDE_DIR})
  list(APPEND BACKEND_LIBRARIES ${SQLITE3_LIBRARIES})
  list(APPEND BACKEND_DEFINITIONS ${SQLITE3_DEFINITIONS})
else(LITESQL_USE_SYSTEM_SQLITE AND SQLITE3_FOUND)
  FetchContent_Declare(libsqlite3
    GIT_REPOSITORY https://github.com/gittiver/sqlite3-amalgamation.git
    GIT_TAG origin/master 
  )
  FetchContent_MakeAvailable(libsqlite3)
#  install(EXPORT sqlite3 DESTINATION lib)


  set(SQLITE3_LIBRARIES sqlite3)  
  message("use internal sqlite3")
  message("- use sqlite3 from: ${SQLITE3_LIBRARIES}")
endif(LITESQL_USE_SYSTEM_SQLITE AND SQLITE3_FOUND)

if (LITESQL_WITH_SQLITE3)
  target_sources(${LIBNAME} 
    PRIVATE  
      sqlite3/sqlite3_backend.cpp)
  target_link_libraries(${LIBNAME} PUBLIC ${SQLITE3_LIBRARIES})

  add_library(${LIBNAME}_sqlite STATIC
              sqlite3/sqlite3_backend.cpp
              sqlite3/sqlite3.hpp)
  target_link_libraries(${LIBNAME}_sqlite PRIVATE ${SQLITE3_LIBRARIES} litesql-util)
  target_include_directories(${LIBNAME}_sqlite PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#  install(TARGETS ${LIBNAME}_sqlite  EXPORT ${LIBNAME}_sqlite
#            RUNTIME DESTINATION lib
#            LIBRARY DESTINATION lib
#            ARCHIVE DESTINATION lib/static
#            COMPONENT devel)
#  install(EXPORT ${LIBNAME}_sqlite DESTINATION lib)
  if (MSVC)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/debug/${LIBNAME}_sqlited.lib DESTINATION lib/static COMPONENT devel)
  endif(MSVC)

  if (LITESQL_WITH_BACKEND_PLUGINS)
    ###########################
    # backend as shared library
    ###########################
    add_library(${LIBNAME}_backend_sqlite3 SHARED 
      sqlite3/sqlite3_backend.cpp 
      sqlite3/sqlite3.hpp sqlite3/sqlite3_backend_plugin.cpp
    )
    target_include_directories(${LIBNAME}_backend_sqlite3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${LIBNAME}_backend_sqlite3 PRIVATE litesql PRIVATE litesql-util PRIVATE ${SQLITE3_LIBRARIES})
  endif()
endif(LITESQL_WITH_SQLITE3)

##############################################################################
# litesql mysql library
##############################################################################
if (LITESQL_WITH_MYSQL)
  target_sources(${LIBNAME} 
  PRIVATE  
    mysql/mysql.cpp)
  target_include_directories(${LIBNAME}
  PRIVATE
    ${MYSQL_INCLUDE_DIR}
  )
  target_link_libraries(${LIBNAME}
  PUBLIC
    ${MYSQL_LIBRARIES}
  )
endif()
if (LITESQL_WITH_MYSQL AND LITESQL_WITH_BACKEND_PLUGINS)
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

endif (LITESQL_WITH_MYSQL AND LITESQL_WITH_BACKEND_PLUGINS)

##############################################################################
# litesql oracle library
##############################################################################
if (LITESQL_WITH_OCILIB)
  target_sources(${LIBNAME} 
  PRIVATE  
    ocilib_backend.cpp)
endif()
if (LITESQL_WITH_OCILIB AND LITESQL_WITH_BACKEND_PLUGINS)
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

endif (LITESQL_WITH_OCILIB AND LITESQL_WITH_BACKEND_PLUGINS)

##############################################################################
# litesql postgres library
##############################################################################
if (LITESQL_WITH_POSTGRES)
  target_sources(${LIBNAME} 
  PRIVATE  
    postgresql.cpp)
endif()
if (LITESQL_WITH_POSTGRES AND LITESQL_WITH_BACKEND_PLUGINS)
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
endif (LITESQL_WITH_POSTGRES AND LITESQL_WITH_BACKEND_PLUGINS)

##############################################################################
# litesql odbc library
##############################################################################
if (LITESQL_WITH_ODBC AND LITESQL_WITH_BACKEND_PLUGINS)
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

endif (LITESQL_WITH_ODBC AND LITESQL_WITH_BACKEND_PLUGINS)

