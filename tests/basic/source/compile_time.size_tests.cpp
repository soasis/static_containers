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

#include <string>

TEMPLATE_LIST_TEST_CASE(
     "fixed_vector no capacity", "[fixed_vector][compile_time][no capacity]", ztd::tests::scalars_list) {
	static_assert(sizeof(ztd::fixed_vector<TestType, 0>) == sizeof(ztd::tests::empty_struct),
	     "Zero-sized fixed_vector should be as small as possible.");
	constexpr ztd::fixed_vector<TestType, 0> vec {};

	static_assert(vec.size() == 0);
	static_assert(vec.capacity() == 0);
	static_assert(vec.empty());
	static_assert(vec.begin() == vec.end());
	static_assert(vec.cbegin() == vec.cend());
	static_assert(vec.data() == nullptr);

	REQUIRE(vec.size() == 0);
	REQUIRE(vec.capacity() == 0);
	REQUIRE(vec.empty());
	REQUIRE(vec.begin() == vec.end());
	REQUIRE(vec.cbegin() == vec.cend());
	REQUIRE(vec.data() == nullptr);
}

TEMPLATE_LIST_TEST_CASE(
     "fixed_vector with capacity with empty", "[fixed_vector][compile_time][empty]", ztd::tests::scalars_list) {
	constexpr std::size_t Capacity = 4;
	constexpr ztd::fixed_vector<TestType, Capacity> vec {};

	static_assert(vec.size() == 0);
	static_assert(vec.capacity() == Capacity);
	static_assert(vec.empty());
	static_assert(vec.begin() == vec.end());
	static_assert(vec.cbegin() == vec.cend());
	static_assert(vec.data() != nullptr);

	REQUIRE(vec.size() == 0);
	REQUIRE(vec.capacity() == Capacity);
	REQUIRE(vec.empty());
	REQUIRE(vec.begin() == vec.end());
	REQUIRE(vec.cbegin() == vec.cend());
	REQUIRE(vec.data() != nullptr);
}
