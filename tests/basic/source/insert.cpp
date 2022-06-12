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

template <typename T, std::size_t Capacity>
constexpr auto make_fixed_vector(std::size_t elements) {
	ztd::fixed_vector<T, Capacity> vec {};
	for (std::size_t i = 0; i < elements; ++i) {
		if constexpr (std::is_integral_v<T>) {
			vec.push_back(T(i));
		}
		else {
			vec.push_back(T {});
		}
	}
	return vec;
}

TEMPLATE_LIST_TEST_CASE(
     "fixed_vector push_back tests", "[fixed_vector][run_time][push_back]", ztd::tests::scalars_list) {
	constexpr std::size_t expected_capacity = 40;
	constexpr std::size_t expected_size     = 5;

	auto vec        = make_fixed_vector<TestType, expected_capacity>(expected_size);
	auto first_it   = vec.begin();
	auto last_it    = vec.end();
	auto rfirst_it  = vec.rbegin();
	auto rlast_it   = vec.rend();
	auto cfirst_it  = vec.cbegin();
	auto clast_it   = vec.cend();
	auto crfirst_it = vec.crbegin();
	auto crlast_it  = vec.crend();

	REQUIRE(vec.size() == expected_size);
	REQUIRE(vec.capacity() == expected_capacity);
	REQUIRE(vec.size());

	REQUIRE(first_it != last_it);
	REQUIRE(cfirst_it != clast_it);
	REQUIRE(rfirst_it != rlast_it);
	REQUIRE(crfirst_it != crlast_it);

	REQUIRE(std::next(first_it, expected_size) == last_it);
	REQUIRE(std::prev(last_it, expected_size) == first_it);
	REQUIRE(std::next(vec.data(), expected_size) == last_it.base());
	REQUIRE(vec.data() == first_it.base());

	REQUIRE(std::next(cfirst_it, expected_size) == clast_it);
	REQUIRE(std::prev(clast_it, expected_size) == cfirst_it);
	REQUIRE(std::next(vec.data(), expected_size) == clast_it.base());
	REQUIRE(vec.data() == cfirst_it.base());

	REQUIRE(std::next(rfirst_it, expected_size) == rlast_it);
	REQUIRE(std::prev(rlast_it, expected_size) == rfirst_it);

	REQUIRE(std::next(crfirst_it, expected_size) == crlast_it);
	REQUIRE(std::prev(crlast_it, expected_size) == crfirst_it);

	if constexpr (std::is_integral_v<TestType>) {
		for (std::size_t i = 0; i < vec.size(); ++i) {
			const auto& value         = vec[i];
			const auto expected_value = TestType(i);
			REQUIRE(value == expected_value);
		}
	}
}
