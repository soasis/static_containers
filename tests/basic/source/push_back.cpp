// =============================================================================
// ztd::fixed_container
//
// Written 2019 - 2022 by ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// ============================================================================ //

#include <ztd/fixed_container.hpp>

#include <catch2/catch_all.hpp>

#include <ztd/tests/types.hpp>

inline namespace ztd_fixed_container_tests_basic_push_back {
	template <typename Container>
	constexpr auto make_fixed_container(std::size_t elements) {
		using T = typename Container::value_type;
		Container c {};
		for (std::size_t i = 0; i < elements; ++i) {
			if ((i % 2) == 1) {
				if constexpr (std::is_arithmetic_v<T>) {
					c.emplace_back(i);
				}
				else {
					c.emplace_back();
				}
			}
			else {
				if constexpr (std::is_arithmetic_v<T>) {
					c.push_back(T(i));
				}
				else {
					c.push_back(T {});
				}
			}
		}
		return c;
	}

	template <typename Container>
	void test_container_push_back(Container& c, std::size_t expected_capacity, std::size_t expected_size) {
		using T = typename ztd::remove_cvref_t<Container>::value_type;

		std::size_t container_capacity_from_function        = c.capacity();
		std::size_t container_capacity_from_member_variable = c.inline_capacity;
		std::size_t container_capacity_from_static_variable = ztd::remove_cvref_t<Container>::inline_capacity;
		REQUIRE(container_capacity_from_function == expected_capacity);
		REQUIRE(container_capacity_from_member_variable == expected_capacity);
		REQUIRE(container_capacity_from_static_variable == expected_capacity);
		auto first_it   = c.begin();
		auto last_it    = c.end();
		auto rfirst_it  = c.rbegin();
		auto rlast_it   = c.rend();
		auto cfirst_it  = c.cbegin();
		auto clast_it   = c.cend();
		auto crfirst_it = c.crbegin();
		auto crlast_it  = c.crend();

		REQUIRE(c.size() == expected_size);
		REQUIRE(c.capacity() == expected_capacity);
		REQUIRE(c.size());

		REQUIRE(first_it != last_it);
		REQUIRE(cfirst_it != clast_it);
		REQUIRE(rfirst_it != rlast_it);
		REQUIRE(crfirst_it != crlast_it);

		REQUIRE(std::next(first_it, expected_size) == last_it);
		REQUIRE(std::prev(last_it, expected_size) == first_it);
		REQUIRE(std::next(c.data(), expected_size) == last_it.base());
		REQUIRE(c.data() == first_it.base());

		REQUIRE(std::next(cfirst_it, expected_size) == clast_it);
		REQUIRE(std::prev(clast_it, expected_size) == cfirst_it);
		REQUIRE(std::next(c.data(), expected_size) == clast_it.base());
		REQUIRE(c.data() == cfirst_it.base());

		REQUIRE(std::next(rfirst_it, expected_size) == rlast_it);
		REQUIRE(std::prev(rlast_it, expected_size) == rfirst_it);

		REQUIRE(std::next(crfirst_it, expected_size) == crlast_it);
		REQUIRE(std::prev(crlast_it, expected_size) == crfirst_it);

		for (std::size_t i = 0; i < c.size(); ++i) {
			const auto& value = c[i];
			if constexpr (std::is_arithmetic_v<T>) {
				const auto expected_value = T(i);
				REQUIRE(value == expected_value);
			}
			else if constexpr (!std::is_class_v<T> && !std::is_union_v<T>) {
				const auto expected_value = T {};
				REQUIRE(value == expected_value);
			}
		}
		if constexpr (ztd::is_fixed_basic_string_v<ztd::remove_cvref_t<Container>>) {
			if constexpr (!(std::is_class_v<T> || std::is_union_v<T>)) {
				const T NullSentinelValue = {};
				auto last_data            = c.data() + c.size();
				auto last_c_str           = c.c_str() + c.size();
				REQUIRE(*last_data == NullSentinelValue);
				REQUIRE(*last_c_str == NullSentinelValue);
			}
		}
	}
} // namespace ztd_fixed_container_tests_basic_push_back

TEMPLATE_LIST_TEST_CASE(
     "fixed_vector push_back tests", "[fixed_vector][run_time][push_back]", ztd::tests::scalar_types_list) {
	constexpr std::size_t expected_capacity = 40;
	constexpr std::size_t expected_size     = 5;

	SECTION("fixed_vector") {
		auto c = make_fixed_container<ztd::fixed_vector<TestType, expected_capacity>>(expected_size);
		test_container_push_back(c, expected_capacity, expected_size);
	}
	SECTION("fixed_basic_string") {
		auto c = make_fixed_container<ztd::fixed_basic_string<TestType, expected_capacity>>(expected_size);
		test_container_push_back(c, expected_capacity, expected_size);
	}
}
