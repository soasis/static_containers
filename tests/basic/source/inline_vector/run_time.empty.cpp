// =============================================================================
// ztd.inline_containers
//
// © ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// ============================================================================ //

#include <ztd/inline_containers.hpp>

#include <catch2/catch_all.hpp>

#include <ztd/tests/types.hpp>

#include <cstddef>
#include <string>

TEMPLATE_LIST_TEST_CASE("inline_vector with an empty with capacity using compile-time capable scalar types",
     "[inline_vector][run_time][empty]", ztd::tests::scalar_types_list) {
	constexpr std::size_t Capacity = 5;
	ztd::inline_vector<TestType, Capacity> c {};
	static_assert(c.capacity() == Capacity);
	auto first_it = c.begin();
	auto last_it  = c.end();
	REQUIRE(c.capacity() == Capacity);
	REQUIRE(c.size() == 0);
	REQUIRE(first_it.base() == c.data());
	c.clear();
	REQUIRE(c.capacity() == Capacity);
	REQUIRE(c.size() == 0);
	REQUIRE(first_it.base() == c.data());
	REQUIRE(last_it.base() == (c.data() + c.size()));
}
