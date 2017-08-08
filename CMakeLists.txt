#***************************************************************************
#                         __    __   _____       _____
#   Project              |  |  |  | |     \     /  ___|
#                        |  |__|  | |  |\  \   /  /
#                        |        | |  | )  ) (  (     
#                        |   /\   | |  |/  /   \  \___
#                         \_/  \_/  |_____/     \_____|
#
# Copyright (C) 2016, The WDC Project, <designerror@yandex.ru>, et al.
#
# This software is licensed as described in the file LICENSE, which
# you should have received as part of this distribution. 
#
# You may opt to use, copy, modify, merge, publish, distribute and/or sell
# copies of the Software, and permit persons to whom the Software is
# furnished to do so, under the terms of the LICENSE file.
#
# This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
# KIND, either express or implied.
#
############################################################################

cmake_minimum_required(VERSION 3.3)

include("gate/cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.61.tar.gz"
    SHA1 "cd4dd406ca45bb6bb28a116d93c2958efac6bf09"
)

project(PYWDC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

hunter_add_package(WDC)
find_package(WDC CONFIG REQUIRED)

hunter_add_package(BOOST COMPONENTS python)
find_package(Boost CONFIG REQUIRED python)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
add_library(pywdc SHARED ${CMAKE_CURRENT_SOURCE_DIR}/pyext.cpp) 
target_link_libraries(pywdc Boost::python ${PYTHON_LIBRARIES} WDC::libwdc)
set_target_properties(pywdc PROPERTIES PREFIX "")
set_target_properties(pywdc PROPERTIES SUFFIX "${CMAKE_SHARED_MODULE_SUFFIX}")
install(TARGETS pywdc DESTINATION bin)
