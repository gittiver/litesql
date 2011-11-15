# - Find OCILIB
# Find the OCILIB includes and client library
# This module defines
#  OCILIB_INCLUDE_DIR, where to find OCILIB.h
#  OCILIB_LIBRARIES, the libraries needed to use OCILIB.
#  OCILIB_FOUND, If false, do not try to use OCILIB.
#
# Copyright (c) 2006, Jaroslaw Staniek, <js@iidea.pl>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if(OCILIB_INCLUDE_DIR AND OCILIB_LIBRARIES)
   set(OCILIB_FOUND TRUE)

else(OCILIB_INCLUDE_DIR AND OCILIB_LIBRARIES)

  # read environment variables and change \ to /
  SET(PROGRAM_FILES_32 $ENV{ProgramFiles})
  STRING(REPLACE "\\\\" "/" PROGRAM_FILES_32 ${PROGRAM_FILES_32})
  
  SET(PROGRAM_FILES_64 $ENV{ProgramW6432})
  if (${PROGRAM_FILES_64})
     STRING(REPLACE "\\\\" "/" PROGRAM_FILES_64 ${PROGRAM_FILES_64})
  endif(${PROGRAM_FILES_64})

  find_path(OCILIB_INCLUDE_DIR OCILIB.h
      /usr/include/OCILIB
      /usr/local/include/OCILIB
      $ENV{ProgramFiles}/OCILIB/*/include
      $ENV{SystemDrive}/OCILIB/*/include
      ${PROGRAM_FILES_32}/OCILIB/*/include
      ${PROGRAM_FILES_64}/OCILIB/*/include
      )

if(WIN32 AND MSVC)
  find_library(OCILIB_LIBRARIES NAMES ociliba
      PATHS
      $ENV{ProgramFiles}/OCILIB/*/lib/opt
      $ENV{SystemDrive}/OCILIB/*/lib/opt
      $ENV{SystemDrive}/OCILIB/*/lib
      ${PROGRAM_FILES_32}/OCILIB/*/lib
      ${PROGRAM_FILES_64}/OCILIB/*/lib
      )
else(WIN32 AND MSVC)
  find_library(OCILIB_LIBRARIES NAMES ocilib
      PATHS
      /usr/lib/OCILIB
      /usr/local/lib/OCILIB
      )
endif(WIN32 AND MSVC)

  if(OCILIB_INCLUDE_DIR AND OCILIB_LIBRARIES)
    set(OCILIB_FOUND TRUE)
    message(STATUS "Found OCILIB: ${OCILIB_INCLUDE_DIR}, ${OCILIB_LIBRARIES}")
  else(OCILIB_INCLUDE_DIR AND OCILIB_LIBRARIES)
    set(OCILIB_FOUND FALSE)
    message(STATUS "OCILIB not found.")
  endif(OCILIB_INCLUDE_DIR AND OCILIB_LIBRARIES)

  mark_as_advanced(OCILIB_INCLUDE_DIR OCILIB_LIBRARIES)

endif(OCILIB_INCLUDE_DIR AND OCILIB_LIBRARIES)
