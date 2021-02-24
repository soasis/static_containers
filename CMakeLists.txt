# ============================================================================
#
# ztd.fixed_vector
#
# Written 2019 - 2021 by ThePhD <phdofthehouse@gmail.com>
#
# To the extent possible under law, the author(s) have dedicated all copyright and related
# and neighboring rights to this software to the public domain worldwide. This software is
# distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
#
# ============================================================================

cmake_minimum_required(VERSION 3.15.0)

# # Project declaration
# informs about the project, gives a description, version and MOST IMPORTANTLY
# the languages the project is going to use. Required.
project(ztd.fixed_vector
	VERSION 0.3.0
	DESCRIPTION "A library for a fixed-capacity vector."
	LANGUAGES C CXX)

# # Modules
# Include modules useful to the project, whether locally made in our own cmake DIRECTORY
# our from the standard cmake libraries
# Add home-rolled modules path to front of module path list
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# # # Top-Level Directories
# Check if this is the top-level project or not
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(ZTD_FIXED_VECTOR_IS_TOP_LEVEL_PROJECT true)
else()
    set(ZTD_FIXED_VECTOR_IS_TOP_LEVEL_PROJECT false)
endif()

# Modify bad flags / change defaults if that is the case
if (ZTD_FIXED_VECTOR_IS_TOP_LEVEL_PROJECT)
	if (CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/${CMAKE_BUILD_TYPE}/${CFG_INT_DIR}/lib")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/${CMAKE_BUILD_TYPE}/${CFG_INT_DIR}/bin")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/${CMAKE_BUILD_TYPE}/${CFG_INT_DIR}/bin")
	else()
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/${CMAKE_BUILD_TYPE}/${CFG_INT_DIR}/lib")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/${CMAKE_BUILD_TYPE}/${CFG_INT_DIR}/bin")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/${CMAKE_BUILD_TYPE}/${CFG_INT_DIR}/bin")
	endif()

	if (MSVC)
		string(REGEX REPLACE "/W[0-4]" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
		string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	endif()
endif()

# # Include standard modules
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# # Options
option(ZTD_FIXED_VECTOR_CI "Whether or not we are in contiguous integration mode" OFF)
option(ZTD_FIXED_VECTOR_TESTS "Enable build of tests" OFF)
option(ZTD_FIXED_VECTOR_EXAMPLES "Enable build of examples" OFF)
option(ZTD_FIXED_VECTOR_BENCHMARKS "Enable build of benchmarks" OFF)

file(GLOB_RECURSE ztd.fixed_vector.sources
	LIST_DIRECTORIES FALSE
	CONFIGURE_DEPENDS
	include/ztd/**.*
)

add_library(ztd.fixed_vector INTERFACE)
add_library(ztd::fixed_vector ALIAS ztd.fixed_vector)
target_sources(ztd.fixed_vector INTERFACE ${ztd.fixed_vector.sources})
target_link_libraries(ztd.fixed_vector)
target_include_directories(ztd.fixed_vector INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
	$<INSTALL_INTERFACE:include>
)
if (ZTD_FIXED_VECTOR_IS_TOP_LEVEL_PROJECT)
	if (MSVC)
		target_compile_options(ztd.fixed_vector INTERFACE 
			/std:c++latest
			/permissive-
		)
	else()
		target_compile_options(ztd.fixed_vector INTERFACE 
			-std=c++2a
		)
	endif()
endif()
#set_target_properties(ztd.fixed_vector
#	PROPERTIES
#	EXPORT_NAME ztd::fixed_vector
#)

# # Benchmarks, Tests, Examples
if (ZTD_FIXED_VECTOR_TESTS)
	enable_testing()
endif()
if (ZTD_FIXED_VECTOR_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()
if (ZTD_FIXED_VECTOR_EXAMPLES)
	add_subdirectory(examples)
endif()
if (ZTD_FIXED_VECTOR_BENCHMARKS)
	add_subdirectory(benchmarks)
endif()
