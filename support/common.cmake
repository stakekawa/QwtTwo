#
# common.cmake for the Qt Two library
#
# Author: Stefano Takekawa s.takekawa_at_gmail.com
#
# Date: 2017
#
# Copyright: Apache Licence 2.0
#


#
# C++ stuff
#
enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


#
# CXXFLAGS
#
set(COMMON_CXX_FLAGS "-std=c++14 -pipe -Wextra -Weffc++ -Wno-system-headers -Wshadow")

set(COMMON_FLAGS_DEBUG "-O0 -g3")
set(COMMON_FLAGS_RELEASE "-O2 -march=native -fomit-frame-pointer")

set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_FLAGS_RELEASE}")

set(CMAKE_CXX_FLAGS "${COMMON_CXX_FLAGS}")


#
# pthread
#
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
