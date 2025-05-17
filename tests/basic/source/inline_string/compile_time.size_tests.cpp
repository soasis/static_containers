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

#include <string>

TEMPLATE_LIST_TEST_CASE(
     "inline_string no capacity", "[inline_string][compile_time][no capacity]", ztd::tests::character_types_list) {
	static_assert(sizeof(ztd::inline_basic_string<TestType, 0>) == sizeof(ztd::tests::empty_struct),
	     "Zero-sized inline_string should be as small as possible.");
	constexpr ztd::inline_basic_string<TestType, 0> c {};
	constexpr TestType NullSentinelValue = TestType {};

	static_assert(c.size() == 0);
	static_assert(c.inline_capacity == 0);
	static_assert(decltype(c)::inline_capacity == 0);
	static_assert(c.capacity() == 0);
	static_assert(c.empty());
	static_assert(c.begin() == c.end());
	static_assert(c.cbegin() == c.cend());
	static_assert(c.data() != nullptr);
	static_assert(*c.data() == NullSentinelValue);
	static_assert(c.c_str() != nullptr);
	static_assert(*c.c_str() == NullSentinelValue);

	REQUIRE(c.size() == 0);
	REQUIRE(c.capacity() == 0);
	REQUIRE(c.empty());
	REQUIRE(c.begin() == c.end());
	REQUIRE(c.cbegin() == c.cend());
	REQUIRE(c.data() != nullptr);
	REQUIRE(*c.data() == NullSentinelValue);
	REQUIRE(c.c_str() != nullptr);
	REQUIRE(*c.c_str() == NullSentinelValue);
}

TEMPLATE_LIST_TEST_CASE("inline_string with capacity with empty", "[inline_string][compile_time][empty]",
     ztd::tests::character_types_list) {
	constexpr std::size_t Capacity = 4;
	constexpr ztd::inline_basic_string<TestType, Capacity> c {};
	constexpr TestType NullSentinelValue = TestType {};

	static_assert(c.size() == 0);
	static_assert(c.inline_capacity == Capacity);
	static_assert(decltype(c)::inline_capacity == Capacity);
	static_assert(c.capacity() == Capacity);
	static_assert(c.empty());
	static_assert(c.begin() == c.end());
	static_assert(c.cbegin() == c.cend());
	static_assert(c.data() != nullptr);
	static_assert(*c.data() == NullSentinelValue);

	REQUIRE(c.size() == 0);
	REQUIRE(c.capacity() == Capacity);
	REQUIRE(c.empty());
	REQUIRE(c.begin() == c.end());
	REQUIRE(c.cbegin() == c.cend());
	REQUIRE(c.data() != nullptr);
	REQUIRE(*c.data() == NullSentinelValue);
}
