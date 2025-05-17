// =============================================================================
// ztd.inline_containers
//
// © ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// ============================================================================ //

#pragma once

#ifndef ZTD_INLINE_CONTAINERS_IS_HPP
#define ZTD_INLINE_CONTAINERS_IS_HPP

#include <ztd/inline_containers/version.hpp>

#include <ztd/inline_containers/forward.hpp>

#include <type_traits>
#include <cstddef>

namespace ztd {
	ZTD_INLINE_CONTAINERS_INLINE_ABI_NAMESPACE_OPEN_I_

	template <typename>
	class is_inline_vector : public ::std::false_type { };

	template <typename _Ty, ::std::size_t _Capacity>
	class is_inline_vector<inline_vector<_Ty, _Capacity>> : public ::std::true_type { };

	template <typename>
	class is_inline_basic_string : public ::std::false_type { };

	template <typename _Ty, ::std::size_t _Capacity>
	class is_inline_basic_string<inline_basic_string<_Ty, _Capacity>> : public ::std::true_type { };

	template <typename _Ty>
	inline constexpr bool is_inline_vector_v = is_inline_vector<_Ty>::value;

	template <typename _Ty>
	inline constexpr bool is_inline_basic_string_v = is_inline_basic_string<_Ty>::value;

	ZTD_INLINE_CONTAINERS_INLINE_ABI_NAMESPACE_CLOSE_I_
} // namespace ztd

#endif
