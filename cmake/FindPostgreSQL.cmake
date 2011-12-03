# - Find PostgreSQL
# Find the PostgreSQL includes and client library
# This module defines
#  POSTGRESQL_INCLUDE_DIR, where to find POSTGRESQL.h
#  POSTGRESQL_LIBRARIES, the libraries needed to use POSTGRESQL.
#  POSTGRESQL_FOUND, If false, do not try to use PostgreSQL.
#
# Copyright (c) 2006, Jaroslaw Staniek, <js@iidea.pl>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
   set(POSTGRESQL_FOUND TRUE)

else(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)

  # read environment variables and change \ to /
  SET(PROGRAM_FILES_32 $ENV{ProgramFiles})
  if (${PROGRAM_FILES_32})
	STRING(REGEX REPLACE "\\\\" "/" PROGRAM_FILES_32 ${PROGRAM_FILES_32})
  	endif(${PROGRAM_FILES_32})
  
  SET(PROGRAM_FILES_64 $ENV{ProgramW6432})
  if (${PROGRAM_FILES_64})
     STRING(REPLACE "\\\\" "/" PROGRAM_FILES_64 ${PROGRAM_FILES_64})
  endif(${PROGRAM_FILES_64})

  find_path(POSTGRESQL_INCLUDE_DIR libpq-fe.h
      /usr/include/pgsql
      /usr/local/include/pgsql
      /usr/include/postgresql
      $ENV{ProgramFiles}/PostgreSQL/*/include
      $ENV{SystemDrive}/PostgreSQL/*/include
      ${PROGRAM_FILES_32}/PostgreSQL/*/include
      ${PROGRAM_FILES_64}/PostgreSQL/*/include
      )

  find_library(POSTGRESQL_LIBRARIES NAMES pq libpq
      PATHS
      /usr/lib
      /usr/local/lib
      /usr/lib/postgresql
      $ENV{ProgramFiles}/PostgreSQL/*/lib/ms
      $ENV{SystemDrive}/PostgreSQL/*/lib/ms
      $ENV{ProgramFiles}/PostgreSQL/*/lib
      $ENV{SystemDrive}/PostgreSQL/*/lib
      ${PROGRAM_FILES_32}/PostgreSQL/*/lib
      ${PROGRAM_FILES_64}/PostgreSQL/*/lib
      )

  if(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
    set(POSTGRESQL_FOUND TRUE)
    message(STATUS "Found PostgreSQL: ${POSTGRESQL_INCLUDE_DIR}, ${POSTGRESQL_LIBRARIES}")
  else(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
    set(POSTGRESQL_FOUND FALSE)
    message(STATUS "PostgreSQL not found.")
  endif(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)

  mark_as_advanced(POSTGRESQL_INCLUDE_DIR POSTGRESQL_LIBRARIES)

endif(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
