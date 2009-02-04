set(LIBNAME xmlreader)

add_library(  ${LIBNAME}
	md5.cpp md5.hpp
	sanitycheck.cpp sanitycheck.cpp
	scanner.cpp scanner.hpp
	stringutils.cpp stringutils.hpp
	xmlreader.cpp
	xmlreader-actions.cpp
	xmltypes.cpp
)

IF (WIN32)
	set_property(TARGET ${LIBNAME} PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS YY_NEVER_INTERACTIVE YY_NO_UNISTD_H YY_NO_INPUT)
	set_property(TARGET ${LIBNAME} PROPERTY SUFFIX .lib)
ENDIF (WIN32)

INCLUDE_DIRECTORIES(../../include ../../win32)
