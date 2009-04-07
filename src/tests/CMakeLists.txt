add_executable(test-selectquery test-selectquery.cpp)
target_link_libraries (test-selectquery litesql)

add_executable(test-split test-split.cpp)
target_link_libraries (test-split litesql)

add_test(test-selectquery selectquery)
add_test(test-split split)

INCLUDE_DIRECTORIES(../library)
