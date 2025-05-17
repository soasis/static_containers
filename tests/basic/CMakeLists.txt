# ============================================================================
#
# ztd.inline_container
#
# © ThePhD <phdofthehouse@gmail.com>
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
file(GLOB_RECURSE ztd.inline_containers.tests.basic.sources
	LIST_DIRECTORIES FALSE
	CONFIGURE_DEPENDS
	source/*.cpp
)

add_executable(ztd.inline_containers.tests.basic ${ztd.inline_containers.tests.basic.sources})
target_compile_definitions(ztd.inline_containers.tests.basic
	PRIVATE
)
target_compile_options(ztd.inline_containers.tests.basic
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
	${--allow-stringop-overflow}
)
target_link_libraries(ztd.inline_containers.tests.basic
	PRIVATE
	ztd.inline_containers
	Catch2::Catch2
	${CMAKE_DL_LIBS}
)
add_test(NAME ztd.inline_containers.tests.basic COMMAND ztd.inline_containers.tests.basic)
