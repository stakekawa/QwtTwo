#
# FindQwt.cmake
#
# Author: Stefano Takekawa s.takekawa_at_gmail.com
#
# Date: 2017
#
# Copyright: Apache Licence 2.0
#

#
# Variables that will be defined
#
# QWT_FOUND       = system has QWT lib
# QWT_LIBRARY     = full path to the QWT library
# QWT_INCLUDE_DIR = where to find headers
#


#
# small log
#
message(" Find Qwt...")
message("")


#
# Qwt Library Names
#
set(QWT_LIBRARY_NAMES qwt-qt5 qwt6-qt5 qwt)


#
# seach for the library
#
find_library(QWT_LIBRARY NAMES ${QWT_LIBRARY_NAMES}
             PATHS /usr/lib /usr/local/lib "$ENV{LIB_DIR}/lib" "$ENV{LIB}" /software/commonDMA/lib)


set(_qwt_fw)


if(QWT_LIBRARY MATCHES "/qwt.*\\.framework")
     string(REGEX REPLACE "^(.*/qwt.*\\.framework).*$" "\\1" _qwt_fw "${QWT_LIBRARY}")
endif()


#
# search for the includes
#
find_path(QWT_INCLUDE_DIR NAMES qwt.h
          PATHS /usr/include /usr/local/include "$ENV{LIB_DIR}/include" "$ENV{INCLUDE}" /software/commonDMA/include
          PATH_SUFFIXES qwt-qt5 qwt qwt6)


#
# check if both includes and library were found
#
string(COMPARE EQUAL "${QWT_INCLUDE_DIR}" "QWT_INCLUDE_DIR-NOTFOUND" QWT_NOTFOUND_INCLUDE)
string(COMPARE EQUAL "${QWT_LIBRARY}" "QWT_LIBRARY-NOTFOUND" QWT_NOTFOUND_LIBRARY)

if(NOT QWT_NOTFOUND_INCLUDE AND NOT QWT_NOTFOUND_LIBRARY)
   set(QWT_FOUND TRUE)
else(NOT QWT_NOTFOUND_INCLUDE AND NOT QWT_NOTFOUND_LIBRARY)
   set(QWT_FOUND FALSE)
endif(NOT QWT_NOTFOUND_INCLUDE AND NOT QWT_NOTFOUND_LIBRARY)


#
# check whether found or not w.r.t. to REQUIRED
#
if(QWT_FOUND)
    FILE(READ ${QWT_INCLUDE_DIR}/qwt_global.h qwt_header)
    STRING(REGEX REPLACE "^.*QWT_VERSION_STR +\"([^\"]+)\".*$" "\\1" QWT_VERSION_STR "${qwt_header}")
    message(STATUS "Found Qwt: ${QWT_LIBRARY} (${QWT_VERSION_STR})")
else(QWT_FOUND)
    if(Qwt_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find Qwt")
    else(Qwt_FIND_REQUIRED)
        message(SEND_ERROR "Could not find Qwt")
    endif(Qwt_FIND_REQUIRED)
    message("")
endif(QWT_FOUND)
