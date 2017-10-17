#
# run_cmake.sh: script to lazily run cmake
#
# Author: Stefano Takekawa s.takekawa_at_gmail.com
#
# Date: 2017
#
# Copyright: Apache Licence 2.0
#


#!/bin/bash

CMAKE_BUILD_TYPE="${1:-Debug}"

echo ""
echo " Build type is ${CMAKE_BUILD_TYPE} ..."
echo ""

INSTALL_DIR=../install

CMAKE_OPT="-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}"

cmake .. ${CMAKE_OPT}  && (echo ""; echo " Done!") || (echo ""; echo " Failed!")

echo ""
