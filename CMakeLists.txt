#
# CMakeLists.txt for the Qwt Two library
#
# Author: Stefano Takekawa s.takekawa_at_gmail.com
#
# Date: 2017
#
# Copyright: Apache Licence 2.0
#


#
# cmake version
#
cmake_minimum_required(VERSION 3.7)


#
# common include
#
include(support/common.cmake)


#
# cmake stuff
#
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/support ${CMAKE_MODULE_PATH})


#
# variables
#
set(PROJECT_BASENAME "QwtTwo")
set(PROJECT_BASEDESCRIPTION "Qwt Two library extensions")
set(PROJECT_BASEDIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(PROJECT_SRC_DIR "${PROJECT_BASEDIR}")
set(PROJECT_BASEVERSION_MAJOR "0")
set(PROJECT_BASEVERSION_MINOR "0")
set(PROJECT_BASEVERSION_PATCH "1")
set(PROJECT_BASEVERSION "${PROJECT_BASEVERSION_MAJOR}.${PROJECT_BASEVERSION_MINOR}.${PROJECT_BASEVERSION_PATCH}")
set(PROJECT_BASESOVERSION "0")
set(PROJECT_INCLUDE_DIR "${PROJECT_BASEDIR}/include")
set(PROJECT_SRC_DIR "${PROJECT_BASEDIR}/src")
set(PROJECT_SUPPORT_DIR "${PROJECT_BASEDIR}/support")


#
# libtwo, QtTwo, QwtTwo
#
set(LIBTWO_DIR "../libtwo/install")
set(LIBQTTWO_DIR "../QtTwo/install")

set(LIBTWO_LIB_DIR "${LIBTWO_DIR}/lib64/")
set(LIBTWO_INCLUDE_DIR "${LIBTWO_DIR}/include/")

set(LIBQTTWO_LIB_DIR "${LIBQTTWO_DIR}/lib64/")
set(LIBQTTWO_INCLUDE_DIR "${LIBQTTWO_DIR}/include/")


#
# project
#
project(${PROJECT_BASENAME} LANGUAGES CXX)


#
# add GNU stuff
#
include(GNUInstallDirs)


#
# Qt stuff
#
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)


#
# Qwt
#
find_package(Qwt REQUIRED)

#
# sources
#
set(PROJECT_SOURCES ${PROJECT_SRC_DIR}/QwtTwoPlot.cpp
                    ${PROJECT_SRC_DIR}/QwtTwoPlotMarker.cpp)

#
# headers
#
set(PROJECT_HEADERS ${PROJECT_INCLUDE_DIR}/QwtTwoPlot.h
                    ${PROJECT_INCLUDE_DIR}/QwtTwoPlotMarker.h)


#
# includes
#
include_directories(/usr/include)
include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${QWT_INCLUDE_DIR})
include_directories(${LIBTWO_INCLUDE_DIR})
include_directories(${LIBQTTWO_INCLUDE_DIR})


#
# libraries
#
find_library(PROJECT_LIBRARY_TWO NAMES two HINTS "${LIBTWO_LIB_DIR}")
find_library(PROJECT_LIBRARY_QTTWO NAMES QtTwo HINTS "${LIBQTTWO_LIB_DIR}")


#
# library configuration
#
add_library(${PROJECT_BASENAME} SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS})

set_target_properties(${PROJECT_BASENAME} PROPERTIES
                      VERSION ${PROJECT_BASEVERSION}
                      SOVERSION ${PROJECT_BASESOVERSION}
                      DESCRIPTION ${PROJECT_BASEDESCRIPTION})

#
# link libraries
#
target_link_libraries(${PROJECT_BASENAME} Threads::Threads
                                          ${QWT_LIBRARY}
                                          ${PROJECT_LIBRARY_QTTWO}
                                          ${PROJECT_LIBRARY_TWO}
                                          Qt5::Core
                                          Qt5::Gui
                                          Qt5::Widgets)


#
# configure files
#
configure_file(${PROJECT_SUPPORT_DIR}/${PROJECT_BASENAME}.pc.in ${PROJECT_SUPPORT_DIR}/${PROJECT_BASENAME}.pc @ONLY)


#
# include stuff
#
target_include_directories(${PROJECT_BASENAME} PRIVATE)


#
# install
#
install(TARGETS ${PROJECT_BASENAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${PROJECT_INCLUDE_DIR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_BASENAME})

install(FILES ${PROJECT_SUPPORT_DIR}/${PROJECT_BASENAME}.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
