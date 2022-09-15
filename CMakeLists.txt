# ============================================================================
#
# ztd.static_containers
#
# Written 2019 - 2022 by ThePhD <phdofthehouse@gmail.com>
#
# To the extent possible under law, the author(s) have dedicated all copyright and related
# and neighboring rights to this software to the public domain worldwide. This software is
# distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
#
# ============================================================================

cmake_minimum_required(VERSION 3.21.0)
cmake_policy(VERSION 3.21)

# # Project kickstart
# Includes a bunch of basic flags and utilities shared across projects
# See more at the github repository link below
include(FetchContent)
FetchContent_Declare(ztd.cmake
	GIT_REPOSITORY https://github.com/soasis/cmake
	GIT_TAG main)
FetchContent_MakeAvailable(ztd.cmake)
set(CMAKE_PROJECT_INCLUDE ${ZTD_CMAKE_PROJECT_PRELUDE})

# # Project declaration
# informs about the project, gives a description, version and MOST IMPORTANTLY
# the languages the project is going to use. Required.
project(ztd.static_containers
	VERSION 0.5.0
	DESCRIPTION "A library for a fixed-capacity vector."
	LANGUAGES C CXX)

if(ZTD_STATIC_CONTAINERS_READTHEDOCS)
	# ReadTheDocs seems unable to handle the include at the project level: something must be going wrong?
	include(CheckCXXCompilerFlag)
	include(CheckCCompilerFlag)
	include(CheckIPOSupported)
	include(CMakePackageConfigHelpers)
	include(CMakeDependentOption)
	include(CMakePrintHelpers)
	include(GNUInstallDirs)
	include(FeatureSummary)
	include(FetchContent)
	include(CTest)
endif()

# # Modules
# Include modules useful to the project, whether locally made in our own cmake DIRECTORY
# our from the standard cmake libraries
# Add home-rolled modules path to front of module path list
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# # # Top-Level Directories
# Check if this is the top-level project or not
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
	set(ZTD_STATIC_CONTAINERS_IS_TOP_LEVEL_PROJECT YES)
else()
	set(ZTD_STATIC_CONTAINERS_IS_TOP_LEVEL_PROJECT NO)
endif()

# Modify bad flags / change defaults if we are the top level
if(ZTD_STATIC_CONTAINERS_IS_TOP_LEVEL_PROJECT)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/${CMAKE_BUILD_TYPE}/lib")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/${CMAKE_BUILD_TYPE}/bin")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/${CMAKE_BUILD_TYPE}/bin")
	else()
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/${CMAKE_BUILD_TYPE}/lib")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/${CMAKE_BUILD_TYPE}/bin")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/${CMAKE_BUILD_TYPE}/bin")
	endif()

	if(NOT DEFINED CMAKE_CXX_STANDARD)
		set(CMAKE_CXX_STANDARD 17)
	endif()

	if(NOT DEFINED CMAKE_C_STANDARD)
		set(CMAKE_C_STANDARD 11)
	endif()

	if(ZTD_STATIC_CONTAINERS_BENCHMARKS OR ZTD_STATIC_CONTAINERS_EXAMPLES OR ZTD_STATIC_CONTAINERS_TESTS OR ZTD_STATIC_CONTAINERS_SCRATCH)
		# normal flags
		check_compiler_flag(disable-permissive MSVC /permissive- GCC -pedantic)

		# Warning flags
		check_compiler_flag(warn-pedantic MSVC /permissive- GCC -pedantic)
		check_compiler_flag(warn-all MSVC /W4 GCC -Wall)
		check_compiler_flag(warn-errors MSVC /WX GCC -Werror)
		check_compiler_flag(warn-extra GCC -Wextra Clang -Wextra)
		check_compiler_flag(utf8-literal-encoding MSVC /execution-charset:utf-8 GCC -fexec-charset=utf-8)
		check_compiler_flag(utf8-source-encoding MSVC /source-charset:utf-8 GCC -finput-charset=utf-8)
		check_compiler_flag(extra-constexpr-depth MSVC /constexpr:depth2147483647 GCC -fconstexpr-depth=2147483647 Clang -fconstexpr-depth=2147483647)
		check_compiler_flag(extra-constexpr-steps MSVC /constexpr:steps2147483647 GCC -fconstexpr-ops-limit=2147483647 Clang -fconstexpr-steps=2147483647)
		check_compiler_flag(template-debugging-mode GCC -ftemplate-backtrace-limit=0)
		check_compiler_diagnostic(zero-variadic-macro-arguments Clang gnu-zero-variadic-macro-arguments)
	endif()
endif()

# # Include standard modules
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# # Options
option(ZTD_STATIC_CONTAINERS_CI "Whether or not we are in contiguous integration mode" OFF)
option(ZTD_STATIC_CONTAINERS_TESTS "Enable build of tests" OFF)
option(ZTD_STATIC_CONTAINERS_EXAMPLES "Enable build of examples" OFF)
option(ZTD_STATIC_CONTAINERS_BENCHMARKS "Enable build of benchmarks" OFF)

# # Dependencies
# ztd.idk
FetchContent_Declare(ztd.idk
	GIT_REPOSITORY https://github.com/soasis/idk.git
	GIT_TAG main)
FetchContent_MakeAvailable(ztd.idk)

file(GLOB_RECURSE ztd.static_containers.sources
	LIST_DIRECTORIES FALSE
	CONFIGURE_DEPENDS
	include/ztd/**.*
)

add_library(ztd.static_containers INTERFACE)
add_library(ztd::static_containers ALIAS ztd.static_containers)
target_sources(ztd.static_containers INTERFACE ${ztd.static_containers.sources})
target_link_libraries(ztd.static_containers
	INTERFACE
	ztd::idk
)
target_include_directories(ztd.static_containers INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

install(DIRECTORY include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# # Config / Version packaging
# Version configurations
configure_package_config_file(
	cmake/ztd.static_containers-config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/ztd.static_containers/ztd.static_containers-config.cmake"
	INSTALL_DESTINATION lib/cmake/ztd.static_containers
	NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/ztd.static_containers/ztd.static_containers-config-version.cmake"
	COMPATIBILITY SameMajorVersion)

export(TARGETS ztd.static_containers
	FILE
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/ztd.static_containers/ztd.static_containers-targets.cmake")

# # Benchmarks, Tests, Examples
if(ZTD_STATIC_CONTAINERS_TESTS)
	enable_testing()
endif()

if(ZTD_STATIC_CONTAINERS_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

if(ZTD_STATIC_CONTAINERS_EXAMPLES)
	add_subdirectory(examples)
endif()

if(ZTD_STATIC_CONTAINERS_BENCHMARKS)
	add_subdirectory(benchmarks)
endif()
