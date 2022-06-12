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

# # Tests
file(GLOB_RECURSE ztd.fixed_vector.tests.basic.sources
	LIST_DIRECTORIES FALSE
	CONFIGURE_DEPENDS
	source/*.cpp
)

add_executable(ztd.fixed_vector.tests.basic ${ztd.fixed_vector.tests.basic.sources})
target_compile_definitions(ztd.fixed_vector.tests.basic
	PRIVATE
)
target_compile_options(ztd.fixed_vector.tests.basic
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
	${--allow-zero-variadic-macro-arguments}
)
target_link_libraries(ztd.fixed_vector.tests.basic
	PRIVATE
	ztd::fixed_vector
	Catch2::Catch2
	${CMAKE_DL_LIBS}
)
add_test(NAME ztd.fixed_vector.tests.basic COMMAND ztd.fixed_vector.tests.basic)
