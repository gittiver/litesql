add_executable(test-selectquery test-selectquery.cpp)
target_link_libraries (test-selectquery litesql litesql-util)
add_test(test-selectquery ${EXECUTABLE_OUTPUT_PATH}/test-selectquery selectquery)

add_executable(test-split test-split.cpp)
target_link_libraries (test-split litesql-util)
add_test(test-split ${EXECUTABLE_OUTPUT_PATH}/test-split split)

add_executable(test-datetime test-datetime.cpp)
target_link_libraries (test-datetime litesql litesql-util)
add_test(test-datetime ${EXECUTABLE_OUTPUT_PATH}/test-datetime datetime)

add_executable(test-update-table test-update-table.cpp)
target_link_libraries (test-update-table litesql litesql-util ${BACKEND_LIBRARIES})
add_test(test-update-table ${EXECUTABLE_OUTPUT_PATH}/test-update-table update-table)

if(WIN32 AND NOT CYGWIN) 
file(TO_NATIVE_PATH ${BACKEND_DL_DIRECTORY} backend_dir)
message("set env for test to:PATH=${backend_dir}")
set_tests_properties(test-update-table PROPERTIES
   ENVIRONMENT "PATH=${backend_dir} "
)
endif(WIN32 AND NOT CYGWIN)
if(APPLE)
set_tests_properties(test-update-table PROPERTIES
  ENVIRONMENT "DYLD_LIBRARY_PATH=${BACKEND_DL_DIRECTORY}"
)
endif(APPLE)
if (UNIX AND NOT APPLE AND NOT WIN32)
set_tests_properties(test-update-table PROPERTIES
  ENVIRONMENT "LD_LIBRARY_PATH=${BACKEND_DL_DIRECTORY}"
)
endif (UNIX AND NOT APPLE AND NOT WIN32)


# backend plugin related tests
if(LITESQL_WITH_BACKEND_PLUGINS)
add_executable(test-backend-loading test-backend-loading.cpp)
target_link_libraries (test-backend-loading litesql litesql-util ${BACKEND_LIBRARIES})
target_include_directories(test-backend-loading PRIVATE ../library)
add_dependencies(test-backend-loading litesql_backend_sqlite3)

add_test(test-backend-loading ${EXECUTABLE_OUTPUT_PATH}/test-backend-loading backend-loading)

if(WIN32 AND NOT CYGWIN)
set_tests_properties(test-backend-loading PROPERTIES
  ENVIRONMENT "PATH=${PATH};${BACKEND_DL_DIRECTORY}"
)
endif(WIN32 AND NOT CYGWIN)
if(APPLE)
set_tests_properties(test-backend-loading PROPERTIES
  ENVIRONMENT "DYLD_LIBRARY_PATH=${BACKEND_DL_DIRECTORY}"
)
endif(APPLE)
if (UNIX AND NOT APPLE AND NOT WIN32)
set_tests_properties(test-backend-loading PROPERTIES
  ENVIRONMENT "LD_LIBRARY_PATH=${BACKEND_DL_DIRECTORY}"
)
endif (UNIX AND NOT APPLE AND NOT WIN32)
endif (LITESQL_WITH_BACKEND_PLUGINS)

