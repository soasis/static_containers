# ============================================================================
#
# ztd.fixed_container
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

file(GLOB_RECURSE ztd.fixed_container.tests.inclusion.sources
	LIST_DIRECTORIES FALSE
	CONFIGURE_DEPENDS
	source/*.cpp
)

add_executable(ztd.fixed_container.tests.inclusion ${ztd.fixed_container.tests.inclusion.sources})
target_compile_options(ztd.fixed_container.tests.inclusion
	PRIVATE
	${--utf8-literal-encoding}
	${--utf8-source-encoding}
	${--disable-permissive}
	${--warn-pedantic}
	${--warn-all}
	${--warn-extra}
	${--warn-errors}
	${--template-debugging-mode}
	${--extra-constexpr-depth}
	${--extra-constexpr-steps}
)
target_link_libraries(ztd.fixed_container.tests.inclusion
	PRIVATE
	ztd::fixed_container
	${CMAKE_DL_LIBS}
)
add_test(NAME ztd.fixed_container.tests.inclusion COMMAND ztd.fixed_container.tests.inclusion)
