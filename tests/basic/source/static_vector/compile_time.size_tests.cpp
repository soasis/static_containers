// =============================================================================
// ztd.static_containers
//
// Written 2019 - 2022 by ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// ============================================================================ //

#include <ztd/static_containers.hpp>

#include <catch2/catch_all.hpp>

#include <ztd/tests/types.hpp>

#include <string>

TEMPLATE_LIST_TEST_CASE(
     "static_vector no capacity", "[static_vector][compile_time][no capacity]", ztd::tests::scalar_types_list) {
	static_assert(sizeof(ztd::static_vector<TestType, 0>) == sizeof(ztd::tests::empty_struct),
	     "Zero-sized static_vector should be as small as possible.");
	constexpr ztd::static_vector<TestType, 0> c {};

	static_assert(c.size() == 0);
	static_assert(c.capacity() == 0);
	static_assert(c.empty());
	static_assert(c.begin() == c.end());
	static_assert(c.cbegin() == c.cend());
	static_assert(c.data() == nullptr);

	REQUIRE(c.size() == 0);
	REQUIRE(c.capacity() == 0);
	REQUIRE(c.empty());
	REQUIRE(c.begin() == c.end());
	REQUIRE(c.cbegin() == c.cend());
	REQUIRE(c.data() == nullptr);
}

TEMPLATE_LIST_TEST_CASE(
     "static_vector with capacity with empty", "[static_vector][compile_time][empty]", ztd::tests::scalar_types_list) {
	constexpr std::size_t Capacity = 4;
	constexpr ztd::static_vector<TestType, Capacity> c {};

	static_assert(c.size() == 0);
	static_assert(c.capacity() == Capacity);
	static_assert(c.empty());
	static_assert(c.begin() == c.end());
	static_assert(c.cbegin() == c.cend());
	static_assert(c.data() != nullptr);

	REQUIRE(c.size() == 0);
	REQUIRE(c.capacity() == Capacity);
	REQUIRE(c.empty());
	REQUIRE(c.begin() == c.end());
	REQUIRE(c.cbegin() == c.cend());
	REQUIRE(c.data() != nullptr);
}
