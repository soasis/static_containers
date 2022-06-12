// =============================================================================
// ztd::fixed_vector
//
// Written 2019 - 2022 by ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// ============================================================================ //

#include <ztd/fixed_vector.hpp>

#include <catch2/catch_all.hpp>

#include <ztd/tests/types.hpp>

#include <cstddef>
#include <string>

TEMPLATE_LIST_TEST_CASE("fixed_vector with an empty with capacity using compile-time capable scalar types",
     "[fixed_vector][run_time][empty]", ztd::tests::scalars_list) {
	constexpr std::size_t Capacity = 5;
	ztd::fixed_vector<TestType, Capacity> vec {};
	static_assert(vec.capacity() == Capacity);
	auto first_it = vec.begin();
	auto last_it  = vec.end();
	REQUIRE(vec.capacity() == Capacity);
	REQUIRE(vec.size() == 0);
	REQUIRE(first_it.base() == vec.data());
	vec.clear();
	REQUIRE(vec.capacity() == Capacity);
	REQUIRE(vec.size() == 0);
	REQUIRE(first_it.base() == vec.data());
	REQUIRE(last_it.base() == (vec.data() + vec.size()));
}
