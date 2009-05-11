add_executable(test-selectquery test-selectquery.cpp)
target_link_libraries (test-selectquery litesql)

add_executable(test-split test-split.cpp)
target_link_libraries (test-split litesql)

add_executable(test-datetime test-datetime.cpp)
target_link_libraries (test-datetime litesql)

add_test(test-selectquery ${EXECUTABLE_OUTPUT_PATH}/test-selectquery selectquery)
add_test(test-split ${EXECUTABLE_OUTPUT_PATH}/test-split split)
add_test(test-datetime ${EXECUTABLE_OUTPUT_PATH}/test-datetime datetime)

INCLUDE_DIRECTORIES(../library)
