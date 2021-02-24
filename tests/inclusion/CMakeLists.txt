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

file(GLOB_RECURSE ztd.fixed_vector.tests.inclusion.sources
	LIST_DIRECTORIES FALSE
	CONFIGURE_DEPENDS
	source/*.cpp
)

add_executable(ztd.fixed_vector.tests.inclusion ${ztd.fixed_vector.tests.inclusion.sources})
if (MSVC)
	target_compile_options(ztd.fixed_vector.tests.inclusion
		PRIVATE /std:c++latest /permissive-)
else()
	target_compile_options(ztd.fixed_vector.tests.inclusion
		PRIVATE -std=c++2a -Wall -Werror -Wpedantic)
endif()
target_link_libraries(ztd.fixed_vector.tests.inclusion
	PRIVATE
	ztd::fixed_vector
	${CMAKE_DL_LIBS}
)
add_test(NAME ztd.fixed_vector.tests.inclusion COMMAND ztd.fixed_vector.tests.inclusion)
