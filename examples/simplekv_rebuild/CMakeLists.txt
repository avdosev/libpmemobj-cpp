# SPDX-License-Identifier: BSD-3-Clause
# Copyright 2019, Intel Corporation

cmake_minimum_required(VERSION 3.3)
project(simplekv_rebuild CXX)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
	pkg_check_modules(LIBPMEMOBJ++ REQUIRED libpmemobj++)
else()
	find_package(LIBPMEMOBJ++ REQUIRED)
endif()

link_directories(${LIBPMEMOBJ++_LIBRARY_DIRS})

add_executable(simplekv_rebuild simplekv_rebuild.cpp)
target_include_directories(simplekv_rebuild PUBLIC ${LIBPMEMOBJ++_INCLUDE_DIRS} . ..)
target_link_libraries(simplekv_rebuild ${LIBPMEMOBJ++_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
