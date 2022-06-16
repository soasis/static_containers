// =============================================================================
// ztd::fixed_container
//
// Written 2019 - 2022 by ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// ============================================================================ //

#pragma once

#ifndef ZTD_FIXED_CONTAINER_IS_HPP
#define ZTD_FIXED_CONTAINER_IS_HPP

#include <ztd/fixed_container/version.hpp>

#include <ztd/fixed_container/forward.hpp>

#include <type_traits>
#include <cstddef>

namespace ztd {
	ZTD_FIXED_CONTAINER_INLINE_ABI_NAMESPACE_OPEN_I_

	template <typename>
	class is_fixed_vector : public ::std::false_type { };

	template <typename _Ty, ::std::size_t _Capacity>
	class is_fixed_vector<fixed_vector<_Ty, _Capacity>> : public ::std::true_type { };

	template <typename>
	class is_fixed_basic_string : public ::std::false_type { };

	template <typename _Ty, ::std::size_t _Capacity>
	class is_fixed_basic_string<fixed_basic_string<_Ty, _Capacity>> : public ::std::true_type { };

	template <typename _Ty>
	inline constexpr bool is_fixed_vector_v = is_fixed_vector<_Ty>::value;

	template <typename _Ty>
	inline constexpr bool is_fixed_basic_string_v = is_fixed_basic_string<_Ty>::value;

	ZTD_FIXED_CONTAINER_INLINE_ABI_NAMESPACE_CLOSE_I_
} // namespace ztd

#endif // ZTD_FIXED_CONTAINER_IS_HPP
