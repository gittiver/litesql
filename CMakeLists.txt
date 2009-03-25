enable_language(C)
enable_language(CXX)

# Include all the necessary files for macros
INCLUDE (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckSymbolExists.cmake)

SET(PROJECT_NAME "litesql")

SET(PROJECT_VERSION_MAJOR 0)
SET(PROJECT_VERSION_MINOR 4)
SET(PROJECT_VERSION_PATCH 0)
SET(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} )

OPTION(WITH_MYSQL "build with mysql Backend" ${MYSQL_FOUND})
OPTION(WITH_SQLITE3 "build with sqlite3 backend(sources integrated)" ON)
OPTION(WITH_POSTGRES "build with postgres backend" OFF)
OPTION(WITH_ODBC "build with ODBC Backend" ON)
OPTION(WITH_DOCS "generate Documentation" OFF)

INCLUDE(cmake/FindMySQL.cmake)
INCLUDE(cmake/FindPostgreSQL.cmake)

IF (WITH_SQLITE3)
  set_property(GLOBAL APPEND PROPERTY COMPILE_DEFINITIONS HAVE_LIBSQLITE3)
ENDIF (WITH_SQLITE3)

IF (WITH_MYSQL AND MYSQL_FOUND)
  set_property(GLOBAL APPEND PROPERTY COMPILE_DEFINITIONS HAVE_LIBMYSQLCLIENT)
ELSE(WITH_MYSQL AND MYSQL_FOUND)
	MESSAGE("can not use mysql")
	SET(WITH_MYSQL OFF)
ENDIF (WITH_MYSQL AND MYSQL_FOUND)

IF (WITH_POSTGRES AND POSTGRESQL_FOUND)
  set_property(GLOBAL APPEND PROPERTY COMPILE_DEFINITIONS HAVE_LIBPQ)
ELSE (WITH_POSTGRES AND POSTGRESQL_FOUND)
	MESSAGE("can not use postgres")
	SET(WITH_POSTGRES OFF)
ENDIF (WITH_POSTGRES AND POSTGRESQL_FOUND)

IF (WITH_ODBC)
  set_property(GLOBAL APPEND PROPERTY COMPILE_DEFINITIONS HAVE_ODBC)
  message(ODBC is not supported yet)
ENDIF (WITH_ODBC)

if (WITH_DOCS)
  add_subdirectory(docs/doxygen)
endif(WITH_DOCS)


IF (WIN32)
	SET(CMAKE_FIND_LIBRARY_PREFIXES "")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
	SET(CMAKE_STATIC_LIBRARY_PREFIX "")
	SET(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
	SET(CMAKE_SHARED_LIBRARY_PREFIX "")          # lib
	SET(CMAKE_SHARED_LIBRARY_SUFFIX ".dll")          # .so
	SET(CMAKE_IMPORT_LIBRARY_PREFIX "")
	SET(CMAKE_IMPORT_LIBRARY_SUFFIX ".lib")
	SET(CMAKE_EXECUTABLE_SUFFIX ".exe")          # .exe
	SET(CMAKE_LINK_LIBRARY_SUFFIX ".lib")
	SET(CMAKE_DL_LIBS "")
	set_property(GLOBAL APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
ENDIF (WIN32)

check_include_files(dlfcn.h HAVE_DLFCN_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(memory.h HAVE_MEMORY_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files(strings.h HAVE_STRINGS_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(unistd.h HAVE_UNISTD_H)

check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists(atof HAVE_ATOF)
check_function_exists(strtoll HAVE_STRTOLL)
check_function_exists(strtof  HAVE_STRTOF)
check_function_exists(localtime_r HAVE_LOCALTIME_R)


# create config.h
CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# set include directories
get_filename_component(CONFIG_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/config.h PATH)
INCLUDE_DIRECTORIES(include ${CONFIG_INCLUDE})

install(DIRECTORY include DESTINATION . FILES_MATCHING PATTERN "*.hpp" PATTERN .svn EXCLUDE)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.h DESTINATION include)

add_subdirectory(src)
 
PROJECT( ${PROJECT_NAME} )

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_GENERATOR TGZ ZIP)
IF(WIN32 AND NOT CYGWIN)
  SET(CPACK_GENERATOR NSIS ${CPACK_GENERATOR})
ENDIF(WIN32 AND NOT CYGWIN)

#SET(CPACK_SOURCE_PACKAGE_FILE_NAME ${PROJECT_NAME}-src-${PROJECT_VERSION})
 
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_NAME})
SET(CPACK_PACKAGE_VENDOR "Me")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
IF(WIN32 AND NOT UNIX)
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}\\\\${PROJECT_VERSION}")
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  #  SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\${PROJECT_NAME}.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} ${PROJECT_NAME}")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.my-project-home-page.org")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.my-personal-home-page.com")
  SET(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")
  SET(CPACK_NSIS_MODIFY_PATH ON)
ELSE(WIN32 AND NOT UNIX)
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}/${PROJECT_VERSION}")
  SET(CPACK_STRIP_FILES "bin/${PROJECT_NAME}-gen")
  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)
SET(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME}-gen ${PROJECT_NAME}-gen)
INCLUDE(CPack)
