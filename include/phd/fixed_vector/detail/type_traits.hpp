//   -----------------
// | phd::fixed_vector |
//   -----------------

// Written in 2019 by ThePhD <phdofthehouse@gmail.com>

// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#pragma once

#ifndef PHD_FIXED_VECTOR_DETAIL_TYPE_TRAITS_HPP
#define PHD_FIXED_VECTOR_DETAIL_TYPE_TRAITS_HPP

#include <utility>
#include <iterator>

namespace phd { namespace __detail { inline namespace __0 {

	template <template <class...> typename Test, typename, typename... Args>
	struct __is_detected_impl : ::std::false_type {};

	template <template <class...> typename Test, typename... Args>
	struct __is_detected_impl<Test, ::std::void_t<Test<Args...>>, Args...> : ::std::true_type {};

	template <template <class...> typename Trait, typename... Args>
	using is_detected = typename __is_detected_impl<Trait, void, Args...>::type;

	template <template <class...> typename Trait, typename... Args>
	constexpr inline bool __is_detected_v = is_detected<Trait, Args...>::value;

}}} // namespace phd::__detail::__0

#endif // PHD_FIXED_VECTOR_DETAIL_TYPE_TRAITS_HPP
