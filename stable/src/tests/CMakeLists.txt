add_executable(test-selectquery test-selectquery.cpp)
target_link_libraries (test-selectquery litesql)

add_executable(test-split test-split.cpp)
target_link_libraries (test-split litesql)

add_test(test-selectquery selectquery)
add_test(test-split split)


#IF (WIN32)
#	set_property(TARGET litesql-gen PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS YY_NEVER_INTERACTIVE YY_NO_UNISTD_H)
#ENDIF (WIN32)


INCLUDE_DIRECTORIES(../../include ../../win32 ../library)
