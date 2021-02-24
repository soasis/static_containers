//   -----------------
// | ztd::fixed_vector |
//   -----------------

// Written 2019 - 2021 by ThePhD <phdofthehouse@gmail.com>

// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#pragma once

#ifndef ZTD_FIXED_VECTOR_DETAIL_TYPE_TRAITS_HPP
#define ZTD_FIXED_VECTOR_DETAIL_TYPE_TRAITS_HPP

#include <ztd/fixed_vector/version.hpp>

#include <utility>
#include <iterator>

namespace ztd {
	ZTD_FIXED_VECTOR_INLINE_ABI_NAMESPACE_OPEN_I_

	namespace __fv_detail {

		template <template <class...> typename Test, typename, typename... Args>
		struct __is_detected_impl : ::std::false_type { };

		template <template <class...> typename Test, typename... Args>
		struct __is_detected_impl<Test, ::std::void_t<Test<Args...>>, Args...> : ::std::true_type { };

		template <template <class...> typename Trait, typename... Args>
		using is_detected = typename __is_detected_impl<Trait, void, Args...>::type;

		template <template <class...> typename Trait, typename... Args>
		constexpr inline bool __is_detected_v = is_detected<Trait, Args...>::value;
	} // namespace __fv_detail

	ZTD_FIXED_VECTOR_INLINE_ABI_NAMESPACE_CLOSE_I_
} // namespace ztd

#endif // ZTD_FIXED_VECTOR_DETAIL_TYPE_TRAITS_HPP
