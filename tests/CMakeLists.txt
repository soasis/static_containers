# ============================================================================
#
# ztd.static_container
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

include(FetchContent)

# # Fetch dependencies
# Catch2
FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG devel
)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(inclusion)
add_subdirectory(basic)
