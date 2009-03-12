enable_language(C)
enable_language(CXX)
SET(PROJECT_NAME "litesql")

SET(PROJECT_VERSION_MAJOR 0)
SET(PROJECT_VERSION_MINOR 4)
SET(PROJECT_VERSION_PATCH 0)
SET(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} )


OPTION(WITH_SQLITE3 "build with sqlite3 backend(sources integrated)" ON)
OPTION(WITH_MYSQL "build with mysql Backend" OFF)
OPTION(WITH_POSTGRES "build with postgres backend" OFF)
OPTION(WITH_ODBC "build with ODBC Backend" ON)
OPTION(WITH_DOCS "generate Documentation" OFF)

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
ENDIF (WIN32)

add_subdirectory(src)
 
# Our project is called 'minimal' this is how it will be called in
    # visual studio, and in our makefiles. 
PROJECT( ${PROJECT_NAME} )
    
# We define the include paths here, our minimal source dir is one, 
INCLUDE_DIRECTORIES(include)


INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_NAME})
SET(CPACK_PACKAGE_VENDOR "Me")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}\\\\${PROJECT_VERSION}")
IF(WIN32 AND NOT UNIX)
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
  SET(CPACK_STRIP_FILES "bin/${PROJECT_NAME}")
  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)
SET(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME} ${PROJECT_NAME})
INCLUDE(CPack)
